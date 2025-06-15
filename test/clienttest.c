#include "../src/conn.h"

int main()
{
    const char *serveraddress = "XX:XX:XX:XX:XX:XX";
    clientConnect(serveraddress, "Hello World");
    return 0;
}