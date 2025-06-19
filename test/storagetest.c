#include "../src/storage.h"

int main()
{
    BluegraphStorage storage = bluegraph_load_storage();
    freeBluegraphStorage(storage);

    return 0;
}