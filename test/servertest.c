#include "../src/conn.h"
#include "../src/storage.h"

int main()
{
    BluegraphStorage storage = bluegraph_load_storage();
    startServer(storage);

    return 0;
}