#include "../src/storage.h"

int main()
{
    //BluegraphStorage storage = bluegraph_load_storage();
    //freeBluegraphStorage(storage);

    MessageFileInfo info = loadMessageInfo("/home/pbjk/.bluegraph/000000000001/1750527580");
    dumpMessageInfo(info, "00:00:00:00:00:01");
    freeMessageInfo(info);

    return 0;
}