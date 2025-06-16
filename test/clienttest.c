#include "../src/conn.h"

int main()
{
    const char *serveraddress = "E8:48:B8:C8:20:00";
    clientConnect(serveraddress, "Hello World");
    return 0;
}