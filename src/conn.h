#ifndef __CONN_H__
#define __CONN_H__

typedef struct
{
    char addr[19];
    char name[248];
} BluegraphDevice_st;

typedef BluegraphDevice_st *BluegraphDevice;

// Discovery
BluegraphDevice *discoverDevices(int len, int *nDevices);
void freeBluegraphDevices(BluegraphDevice *devices, int nDevices);

// Server
void startServer();
// Client
void clientConnect(const char *serverAddress, const char *msg);

#define MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER 255

// TODO: This shall be updated
#define BLUEGRAPH_DEFAULT_PORT 5

#endif