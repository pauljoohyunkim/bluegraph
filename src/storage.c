#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include "storage.h"

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
        if (!S_ISDIR(filestat.st_mode))
            continue;
        printf("%s\n", filename);
        free(filename);
    }
    closedir(dp);
    
    return storage;
}

void freeBluegraphStorage(BluegraphStorage storage)
{
    if (!storage) return;

    free(storage->dir);
    free(storage);
}