#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include "storage.h"

static bool isCompressedBDAddress(char *dirname);

// Helper function for checking the format XXxxXXxxXXxx
static bool isCompressedBDAddress(char *dirname)
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

// TODO: Get time value.
// Given full filename, returns MessageFileInfo of the message.
MessageFileInfo loadMessageInfo(char *filename)
{
    MessageFileInfo info = NULL;
    FILE *fp = NULL;
    size_t bytes = 0;
    size_t filesize = 0;
    char *epochstr = NULL;
    time_t epoch = 0;

    fp = fopen(filename, "r");
    if (fp == NULL) return NULL;

    // epochstr is the basename for the filename given.
    epochstr = strrchr(filename, '/');
    if (!epochstr) return NULL;
    epochstr += 1;
    // TODO: parse epochstr to time value.
    epoch = strtoll(epochstr, NULL, 10);

    // Determine file size
    fseek(fp, 0, SEEK_END);
    filesize = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    info = calloc(1, sizeof(MessageFileInfo_st));
    // Read the first two bytes to determine its direction, and whether it is a text message.
    bytes = fread(&(info->direction), sizeof(uint8_t), 1, fp);
    bytes = fread(&(info->isText), sizeof(uint8_t), 1, fp);
    info->time = epoch;
    if (bytes == 0 || (info->direction != BLUEGRAPH_INCOMING && info->direction != BLUEGRAPH_OUTGOING))
    {
        fclose(fp);
        freeMessageInfo(info);
        return NULL;
    }

    info->info = calloc(filesize, sizeof(char));
    bytes = fread(info->info, 1, filesize, fp);
    fclose(fp);

    return info;
}

void dumpMessageInfo(MessageFileInfo info, char *bdaddr)
{
    time_t time;
    struct tm *datetime;

    if (!info) return;

    datetime = localtime(&time);
    printf("%s", asctime(datetime));

    if (info->direction == BLUEGRAPH_INCOMING)
    {
        printf("%s -> Me\n", bdaddr);
    }
    else
    {
        printf("Me -> %s\n", bdaddr);
    }

    if (info->isText)
    {
        printf("%s\n", info->info);
    }
    else
    {
        // TODO: Show link to the file path.
    }
}

void writeMessageInfo(MessageFileInfo info, char *bdaddr_dirname)
{
    FILE *fp = NULL;
    char *messageFilename = NULL;
    char epochstr[21] = { 0 };
    struct stat st = { 0 };
    if (!info) return;

    if (stat(bdaddr_dirname, &st) == -1)
    {
        fprintf(stderr, "Creating %s\n", bdaddr_dirname);
        mkdir(bdaddr_dirname, 0700);
    }

    snprintf(epochstr, sizeof(epochstr), "%llu", info->time);

    messageFilename = calloc(sizeof(uint64_t) + strlen(bdaddr_dirname) + 2, sizeof(uint8_t));
    strcpy(messageFilename, bdaddr_dirname);
    strcat(messageFilename, "/");
    strcat(messageFilename, epochstr);

    fp = fopen(messageFilename, "wb");
    if (!fp)
    {
        free(messageFilename);
        return;
    }

    // Write the first two bytes to encode information on direction and if it's a text.
    fwrite((uint8_t *) &(info->direction), sizeof(uint8_t), 1, fp);
    fwrite((uint8_t *) &(info->isText), sizeof(uint8_t), 1, fp);
    fwrite(info->info, sizeof(uint8_t), strlen(info->info), fp);
    fclose(fp);
    free(messageFilename);
}

void freeMessageInfo(MessageFileInfo info)
{
    free(info->info);
    free(info);
}

// Pass in the full path to bdaddr directory.
// Loads chat for a single Bluetooth device.
BluegraphChat loadBluegraphChat(char *bdaddr_dirname)
{
    BluegraphChat chat = NULL;
    char *bdaddr = NULL;
    DIR *dp = NULL;
    struct dirent *op = NULL;

    // bdaddr is the basename for bdaddr_dirname
    bdaddr = strrchr(bdaddr_dirname, '/');
    if (!bdaddr) return NULL;
    bdaddr += 1;

    chat = calloc(1, sizeof(BluegraphChat_st));
    chat->contact = calloc(1, sizeof(BluegraphDevice_st));
    compressedBDAddress2StringAddress(chat->contact->addr, bdaddr);
    // TODO: For the name, check remotely, and if it fails, used cached name.
    
    // Reading through the bdaddr_dirname directory.
    chat->chatfiles = createFileList();
    dp = opendir(bdaddr_dirname);
    while ((op = readdir(dp)) != NULL)
    {
        struct stat filestat = { 0 };
        char *filename = NULL;
        if (strcmp(op->d_name, ".") == 0 || strcmp(op->d_name, "..") == 0)
            continue;
        filename = calloc(strlen(op->d_name) + strlen(bdaddr_dirname) + 2, sizeof(char));
        strcpy(filename, bdaddr_dirname);
        strcat(filename, "/");
        strcat(filename, op->d_name);
        stat(filename, &filestat);
        addToFileList(chat->chatfiles, op->d_name);
        free(filename);
    }
    closedir(dp);
}

void dumpChat(char *bdaddr_dirname)
{
    char *bdaddrCompressed = NULL;
    char bdaddr[18] = { 0 };
    DIR *dp = NULL;
    struct dirent *op = NULL;

    bdaddrCompressed = strrchr(bdaddr_dirname, '/');
    if (!bdaddrCompressed) return;
    bdaddrCompressed += 1;
    compressedBDAddress2StringAddress(bdaddr, bdaddrCompressed);

    // Get file list, then dump each message.
    dp = opendir(bdaddr_dirname);
    while ((op = readdir(dp)) != NULL)
    {
        char *filename = NULL;
        MessageFileInfo info = NULL;
        if (strcmp(op->d_name, ".") == 0 || strcmp(op->d_name, "..") == 0)
            continue;
        filename = calloc(strlen(op->d_name) + strlen(bdaddr_dirname) + 2, sizeof(char));
        strcpy(filename, bdaddr_dirname);
        strcat(filename, "/");
        strcat(filename, op->d_name);

        info = loadMessageInfo(filename);
        dumpMessageInfo(info, bdaddr);
        freeMessageInfo(info);
        free(filename);
    }
    closedir(dp);
}

void freeBluegraphChat(BluegraphChat chat)
{
    if (!chat) return;

    freeFileList(chat->chatfiles);
    free(chat);
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
    storage->chatdirs = createFileList();
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
        if (!S_ISDIR(filestat.st_mode) || !isCompressedBDAddress(op->d_name))
        {
            free(filename);
            continue;
        }
        //printf("%s\n", filename);
        addToFileList(storage->chatdirs, op->d_name);
        loadBluegraphChat(filename);
        free(filename);
    }
    closedir(dp);
    
    return storage;
}

void freeBluegraphStorage(BluegraphStorage storage)
{
    if (!storage) return;

    free(storage->dir);
    freeFileList(storage->chatdirs);
    free(storage);
}

void compressedBDAddress2StringAddress(char *stringAddress, char *compressedBDAddress)
{
    if (!stringAddress || !compressedBDAddress) return;
    if (!isCompressedBDAddress(compressedBDAddress)) return;

    memset(stringAddress, 0, 18);
    strncpy(stringAddress, compressedBDAddress, 2);
    for (int i = 1; i < 6; i++)
    {
        strcat(stringAddress, ":");
        strncat(stringAddress, compressedBDAddress + 2 * i, 2);
    }
}

void stringAddress2CompressedBDAddress(char *compressedBDAddress, char *stringAddress)
{
    int i = 0;
    int j = 0;
    if (!stringAddress || !compressedBDAddress) return;

    memset(compressedBDAddress, 0, 13);

    while (i < 18)
    {
        if (stringAddress[i] == ':')
        {
            i++;
            continue;
        }
        compressedBDAddress[j] = stringAddress[i];
        j++;
        i++;
    }
}