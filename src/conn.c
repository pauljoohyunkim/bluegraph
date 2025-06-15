#include <stdlib.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include "conn.h"

// Creates List of BluegraphDevice
// Inquires for 1.28 * len seconds.
BluegraphDevice *discoverDevices(int len, int *nDevices)
{
    BluegraphDevice* deviceList = NULL;
    inquiry_info* ii = NULL;
    int dev_id, sock;
    const int maxDevices = MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER;
    char addr[100];     // TODO: Testing

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