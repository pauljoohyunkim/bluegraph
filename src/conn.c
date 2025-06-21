#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include "capsule.h"
#include "conn.h"
#include "storage.h"
#include "transaction.h"

// Creates List of BluegraphDevice
// Inquires for 1.28 * len seconds.
BluegraphDevice *discoverDevices(int len, int *nDevices)
{
    BluegraphDevice* deviceList = NULL;
    inquiry_info* ii = NULL;
    int dev_id, sock;
    const int maxDevices = MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER;

    // Open Bluetooth adapter
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) return NULL;

    // Inquire devices
    ii = (inquiry_info*) calloc(maxDevices, sizeof(inquiry_info));
    *nDevices = hci_inquiry(dev_id, len, maxDevices, NULL, &ii, IREQ_CACHE_FLUSH);
    if (*nDevices < 0)
    {
        // TODO: Error
        close(sock);
        free(ii);
        return NULL;
    }
    else if (*nDevices == 0)
    {
        // TODO: No devices found
        close(sock);
        free(ii);
        return NULL;
    }
    else
    {
        deviceList = (BluegraphDevice*) calloc(*nDevices, sizeof(BluegraphDevice));
        for (int i = 0; i < *nDevices; i++)
        {
            deviceList[i] = (BluegraphDevice) calloc(1, sizeof(BluegraphDevice_st));
        }
    }

    for (int i = 0; i < *nDevices; i++)
    {
        ba2str(&(ii[i].bdaddr), deviceList[i]->addr);
        memset(deviceList[i]->name, 0, sizeof(deviceList[i]->name));
        if (hci_read_remote_name(sock, &(ii[i].bdaddr), sizeof(deviceList[i]->name), deviceList[i]->name, 0) < 0)
        {
            strcpy(deviceList[i]->name, "[Unknown Device]");
        }
    }

    free(ii);
    close(sock);
    return deviceList;
}

void freeBluegraphDevices(BluegraphDevice *devices, int nDevices)
{
    if (!devices || nDevices == 0) return;

    for (int i = 0; i < nDevices; i++)
    {
        free(devices[i]);
    }
    free(devices);
}

void startServer(BluegraphStorage storage, char *bdaddr)
{
    struct sockaddr_rc loc_addr = { 0 };
    int s;
    fd_set currentDescriptors;
    fd_set readyDescriptors;
    
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // Bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) BLUEGRAPH_DEFAULT_PORT;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    listen(s, 1);

    FD_ZERO(&currentDescriptors);
    FD_SET(STDIN_FILENO, &currentDescriptors);
    FD_SET(s, &currentDescriptors);
    
    while (1)
    {
        readyDescriptors = currentDescriptors;
        if (select(FD_SETSIZE, &readyDescriptors, NULL, NULL, NULL) < 0)
        {
            fprintf(stderr, "Select error.\n");
            continue;
        }
        for (int i = 0; i < FD_SETSIZE; i++)
        {
            if (FD_ISSET(i, &readyDescriptors))
            {
                if (i == s)
                {
                    serverTransaction(s, storage);
                }
                if (i == STDIN_FILENO)
                {
                    Transaction transaction = NULL;
                    MessageFileInfo info = NULL;
                    char buf[BUFSIZ];
                    char compressedBDAddr[12];
                    char *filename = NULL;

                    read(STDIN_FILENO, buf, BUFSIZ);
                    printf("%s\n", buf);
                    transaction = createTransaction();
                    transaction->type = BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE;
                    transaction->send_message_info.sourceType = BLUEGRAPH_MESSAGE_SOURCE_BUFFER;
                    transaction->send_message_info.messageLen = strlen(buf);
                    clientConnect(bdaddr, transaction);
                    info = calloc(1, sizeof(MessageFileInfo_st));
                    info->time = time(NULL);
                    info->direction = BLUEGRAPH_OUTGOING;
                    info->isText = true;
                    info->info = calloc(strlen(buf), 1);
                    filename = calloc(strlen(storage->dir) + 2 + 12, sizeof(char));
                    stringAddress2CompressedBDAddress(compressedBDAddr, bdaddr);
                    strcpy(filename, storage->dir);
                    strcat(filename, "/");
                    strcat(filename, compressedBDAddr);
                    writeMessageInfo(info, filename);
                    free(filename);
                    freeMessageInfo(info);
                    free(transaction);

                    memset(buf, 0, sizeof(buf));
                }
            }
        }
            
    }
    
    close(s);
}

// TODO: In the msg, there will be a flag encoded such that
// the segment sent is the last chunk.
void clientConnect(const char *serverAddress, Transaction transaction)
{
    struct sockaddr_rc addr = {0};
    int s, status;

    // allocate a socket
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
    // set the connection parameters (who to connect to)
    addr.rc_family = AF_BLUETOOTH;
    addr.rc_channel = (uint8_t) BLUEGRAPH_DEFAULT_PORT;
    str2ba(serverAddress, &addr.rc_bdaddr);
    
    status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
    // send a message
    if (status == 0)
    {   
        status = clientTransaction(transaction, s);
    }
    if (status < 0) fprintf(stderr, "Client transaction failed.\n");
    close(s);
}