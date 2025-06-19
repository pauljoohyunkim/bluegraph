#ifndef __STORAGE_H__
#define __STORAGE_H__

typedef struct
{
    char *dir;
} BluegraphStorage_st;

typedef BluegraphStorage_st *BluegraphStorage;

BluegraphStorage bluegraph_load_storage();
void freeBluegraphStorage(BluegraphStorage storage);

#endif