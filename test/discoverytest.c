#include "../src/conn.h"

int main()
{
    int nDevices = 0;
    BluegraphDevice *devices = discoverDevices(5, &nDevices);

    freeBluegraphDevices(devices, nDevices);
    return 0;
}