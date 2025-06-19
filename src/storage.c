#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include "storage.h"

static bool isCompressedBAAddress(char *dirname);

// Helper function for checking the format XXxxXXxxXXxx
static bool isCompressedBAAddress(char *dirname)
{
    if (strlen(dirname) != 12)
        return false;
    for (int i = 0; i < 12; i++)
    {
        if ((dirname[i] < 'A' || dirname[i] > 'F') && (dirname[i] < '0' || dirname[i] > '9'))
            return false;
    }
    return true;
}

FileList createFileList()
{
    FileList filelist = calloc(1, sizeof(FileList_st));
    filelist->size = 0;
    filelist->filenames = calloc(1, sizeof(char*));
    filelist->capacity = 1;

    return filelist;
}

void addToFileList(FileList filelist, char *filename)
{
    // TODO: Implement this.
    if (!filelist) return;

    if (filelist->size == filelist->capacity)
    {
        filelist->capacity *= 2;
        filelist->filenames = realloc(filelist->filenames, filelist->capacity * sizeof(char*));
    }
    filelist->filenames[filelist->size] = calloc(strlen(filename) + 1, sizeof(char));
    strcpy(filelist->filenames[filelist->size], filename);
    filelist->size++;
}

void freeFileList(FileList filelist)
{
    if (!filelist) return;

    for (size_t i = 0; i < filelist->size; i++)
    {
        free(filelist->filenames[i]);
    }
    free(filelist->filenames);
    free(filelist);
}

BluegraphStorage bluegraph_load_storage()
{
    BluegraphStorage storage = NULL;
    char *homedir = NULL;
    const char bluegraphdir[] = "/.bluegraph";
    struct stat st = { 0 };
    DIR *dp = NULL;
    struct dirent *op = NULL;

    homedir = getenv("HOME");       // TODO: Make this cross-platform.
    if (!homedir)
    {
        fprintf(stderr, "Could not find HOME environment variable.\n");
        return NULL;
    }

    storage = calloc(1, sizeof(BluegraphStorage_st));
    if (!storage)
    {
        fprintf(stderr, "Could not allocate storage memory for storage information.\n");
        free(homedir);
        return NULL;
    }

    storage->dir = calloc(strlen(homedir) + strlen(bluegraphdir) + 1, sizeof(char));
    strcpy(storage->dir, homedir);
    strcat(storage->dir, bluegraphdir);

    // If the directory does not exist, create one.
    if (stat(storage->dir, &st) == -1)
    {
        fprintf(stderr, "Creating %s\n", storage->dir);
        mkdir(storage->dir, 0700);
    }

    // Read through the bluegraph rootdir
    // Each directory should be XXxxXXxxXXxx
    // where it is associated with bdaddr XX:xx:XX:xx:XX:xx
    storage->chats = createFileList();
    dp = opendir(storage->dir);
    while ((op = readdir(dp)) != NULL)
    {
        struct stat filestat = { 0 };
        char *filename = NULL;
        if (strcmp(op->d_name, ".") == 0 || strcmp(op->d_name, "..") == 0)
            continue;
        filename = calloc(strlen(op->d_name) + strlen(storage->dir) + 2, sizeof(char));
        strcpy(filename, storage->dir);
        strcat(filename, "/");
        strcat(filename, op->d_name);
        stat(filename, &filestat);
        if (!S_ISDIR(filestat.st_mode) || !isCompressedBAAddress(op->d_name))
        {
            free(filename);
            continue;
        }
        printf("%s\n", filename);
        addToFileList(storage->chats, op->d_name);
        free(filename);
    }
    closedir(dp);
    
    return storage;
}

void freeBluegraphStorage(BluegraphStorage storage)
{
    if (!storage) return;

    free(storage->dir);
    freeFileList(storage->chats);
    free(storage);
}