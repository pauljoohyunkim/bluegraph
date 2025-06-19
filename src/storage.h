#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <stdlib.h>

typedef struct
{
    char **filenames;
    size_t size;
    size_t capacity;
} FileList_st;

typedef FileList_st *FileList;

typedef struct
{
    char *dir;
    FileList chatdirs;
} BluegraphStorage_st;

typedef BluegraphStorage_st *BluegraphStorage;

FileList createFileList();
void addToFileList(FileList filelist, char *filename);
void freeFileList(FileList filelist);
BluegraphStorage bluegraph_load_storage();
void freeBluegraphStorage(BluegraphStorage storage);

#endif