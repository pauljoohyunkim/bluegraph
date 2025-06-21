#include "../src/storage.h"

int main()
{
    BluegraphStorage storage = bluegraph_load_storage();
    dumpChat("/home/pbjk/.bluegraph/000000000001");
    freeBluegraphStorage(storage);

    return 0;
}