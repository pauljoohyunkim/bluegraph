#ifndef __CONN_H__
#define __CONN_H__

#include "device.h"
#include "transaction.h"

struct BluegraphStorage_st;
typedef struct BluegraphStorage_st *BluegraphStorage;

// Declaration

// Discovery
BluegraphDevice *discoverDevices(int len, int *nDevices);
void freeBluegraphDevices(BluegraphDevice *devices, int nDevices);

// Server
void startServer(BluegraphStorage storage, char *bdaddr);
// Client
void clientConnect(const char *serverAddress, Transaction transaction);

#define MAX_BLUETOOTH_DISCOVERY_DEVICE_NUMBER 255
#define BLUEGRAPH_CHUNK_SIZE 1024

// TODO: This shall be updated
#define BLUEGRAPH_DEFAULT_PORT 5

#endif