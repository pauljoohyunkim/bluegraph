#ifndef __CONN_H__
#define __CONN_H__

typedef struct
{
    char addr[19];
    char name[248];
} BluegraphDevice_st;

typedef BluegraphDevice_st *BluegraphDevice;

BluegraphDevice *discoverDevices(int len, int *nDevices);
void freeBluegraphDevices(BluegraphDevice *devices, int nDevices);

#define MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER 255

#endif