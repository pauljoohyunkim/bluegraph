#ifndef __STORAGE_H__
#define __STORAGE_H__

#include <stdlib.h>
#include <time.h>
#include "device.h"
#include "common.h"
#include "conn.h"


// Declaration
struct FileList_st;
typedef struct FileList_st FileList_st;
typedef struct FileList_st *FileList;

struct BluegraphChat_st;
typedef struct BluegraphChat_st BluegraphChat_st;
typedef struct BluegraphChat_st *BluegraphChat;

struct MessageFileInfo_st;
typedef struct MessageFileInfo_st MessageFileInfo_st;
typedef struct MessageFileInfo_st *MessageFileInfo;

struct FileList_st
{
    char **filenames;
    size_t size;
    size_t capacity;
};

struct BluegraphStorage_st
{
    char *dir;
    FileList chatdirs;
};

struct BluegraphChat_st
{
    BluegraphDevice contact;
    FileList chatfiles;
};

typedef enum
{
    BLUEGRAPH_INCOMING,
    BLUEGRAPH_OUTGOING
} MessageDirection;

struct MessageFileInfo_st
{
    time_t time;
    MessageDirection direction;
    bool isText;
    // If isText == true, then info is the content of the message.
    // Otherwise it is the name of the file.
    char *info;
};

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
void stringAddress2CompressedBDAddress(char *compressedBDAddress, char *stringAddress);

#endif