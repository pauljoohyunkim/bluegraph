#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <bluetooth/rfcomm.h>
#include "capsule.h"
#include "conn.h"
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

void startServer()
{
    struct sockaddr_rc loc_addr = {0}, rem_addr = {0};
    uint8_t buf[BLUEGRAPH_CHUNK_SIZE] = {0};
    int s, client;
    int opt = sizeof(rem_addr);
    
    s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);

    // Bluetooth adapter
    loc_addr.rc_family = AF_BLUETOOTH;
    loc_addr.rc_bdaddr = *BDADDR_ANY;
    loc_addr.rc_channel = (uint8_t) BLUEGRAPH_DEFAULT_PORT;
    bind(s, (struct sockaddr *)&loc_addr, sizeof(loc_addr));
    listen(s, 1);
    
    while (1)
    {
        int bytes_read = 0;
        Capsule clientCapsule = NULL;
        Capsule serverCapsule = NULL;
        Packet serverPacket = NULL;
        size_t serverPacketSize = 0;

        client = accept(s, (struct sockaddr *)&rem_addr, &opt);
        ba2str(&rem_addr.rc_bdaddr, (char *) buf);
        fprintf(stderr, "accepted connection from %s\n", buf);
        memset(buf, 0, sizeof(buf));

        bytes_read = read(client, buf, BLUEGRAPH_CHUNK_SIZE);
        if (bytes_read > 0)
        {
            clientCapsule = packet2capsule(buf, bytes_read);
            if (!clientCapsule) continue;

            switch (clientCapsule->type)
            {
                case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
                    // TODO: For now, send positive ack for all messages.
                    serverCapsule = createCapsule();
                    serverCapsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK;
                    serverCapsule->send_message_request_ack_info.ack = true;
                    serverPacket = capsule2packet(serverCapsule, &serverPacketSize);
                    freeCapsule(serverCapsule);

                    write(client, serverPacket, serverPacketSize);
                    free(serverPacket);

                    // TODO: Getting packet from client
                    bytes_read = read(client, buf, BLUEGRAPH_CHUNK_SIZE);
                    clientCapsule = packet2capsule(buf, bytes_read);

                    freeCapsule(clientCapsule);
                    break;
                default:
                    break;
            }
            printf("received [%s]\n", buf);
        }
    }
    
    close(client);
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