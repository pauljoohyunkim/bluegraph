#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <stdlib.h>
#include "conn.h"

// Creates List of 255 BluegraphDevice
// Inquires for 1.28 * len seconds.
BluegraphDevice *discoverDevices(int len, int *nDevices)
{
    BluegraphDevice* deviceList = NULL;
    inquiry_info** ii = NULL;
    int dev_id, sock;

    // Open Bluetooth adapter
    dev_id = hci_get_route(NULL);
    sock = hci_open_dev(dev_id);
    if (dev_id < 0 || sock < 0) return NULL;

    // Inquire devices
    ii = (inquiry_info**) calloc(MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER, sizeof(inquiry_info));
    *nDevices = hci_inquiry(dev_id, len, MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER, NULL, ii, IREQ_CACHE_FLUSH);
    if (*nDevices < 0)
    {
        close(sock);
        free(ii);
        return NULL;
    }

    // TODO: Get Remote Names
}