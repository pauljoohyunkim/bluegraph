#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "storage.h"

BluegraphStorage bluegraph_load_storage()
{
    BluegraphStorage storage = NULL;
    char *homedir = NULL;
    const char bluegraphdir[] = "/.bluegraph";
    struct stat st;

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
    return storage;
}

void freeBluegraphStorage(BluegraphStorage storage)
{
    if (!storage) return;

    free(storage->dir);
    free(storage);
}