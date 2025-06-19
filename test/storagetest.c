#include "../src/storage.h"

int main()
{
    //BluegraphStorage storage = bluegraph_load_storage();
    //freeBluegraphStorage(storage);

    MessageFileInfo info = loadMessageInfo("/home/pbjk/.bluegraph/123456789012/test.txt");
    freeMessageInfo(info);

    return 0;
}