#ifndef __CONN_H__
#define __CONN_H__

#include "storage.h"
#include "transaction.h"

typedef struct
{
    char addr[18];
    char name[248];
} BluegraphDevice_st;

typedef BluegraphDevice_st *BluegraphDevice;

// Discovery
BluegraphDevice *discoverDevices(int len, int *nDevices);
void freeBluegraphDevices(BluegraphDevice *devices, int nDevices);

// Server
void startServer(BluegraphStorage storage);
// Client
void clientConnect(const char *serverAddress, Transaction transaction);

#define MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER 255
#define BLUEGRAPH_CHUNK_SIZE 1024

// TODO: This shall be updated
#define BLUEGRAPH_DEFAULT_PORT 5

#endif