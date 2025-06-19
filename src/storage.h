#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <stdlib.h>
#include <time.h>
#include "conn.h"

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

typedef struct
{
    BluegraphDevice contact;
    FileList chatfiles;
} BluegraphChat_st;
typedef BluegraphChat_st *BluegraphChat;

typedef enum
{
    BLUEGRAPH_INCOMING,
    BLUEGRAPH_OUTGOING
} MessageDirection;

typedef struct
{
    time_t time;
    MessageDirection direction;
    bool isText;
    // If isText == true, then info is the content of the message.
    // Otherwise it is the name of the file.
    char *info;
} MessageFileInfo_st;
typedef MessageFileInfo_st *MessageFileInfo;

FileList createFileList();
void addToFileList(FileList filelist, char *filename);
void freeFileList(FileList filelist);

MessageFileInfo loadMessageInfo(char *filename);
void writeMessageInfo(MessageFileInfo info, char *bdaddr_dirname);
void freeMessageInfo(MessageFileInfo info);

BluegraphChat loadBluegraphChat(char *bdaddr_dirname);
void freeBluegraphChat(BluegraphChat chat);

BluegraphStorage bluegraph_load_storage();
void freeBluegraphStorage(BluegraphStorage storage);

void compressedBDAddress2StringAddress(char *stringAddress, char *compressedBDAddress);

#endif