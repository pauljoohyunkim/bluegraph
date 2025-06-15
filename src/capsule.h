#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <cjson/cJSON.h>

typedef enum
{
    BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE,    // Sending message
    BLUEGRAPH_CAPSULE_TYPE_SEND_RECEIPT,    // Acknowledgment of receiving message
    BLUEGRAPH_CAPSULE_TYPE_QUERY,           // Querying if node can reach destination.
    BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY      // Reply to the query
} CapsuleType;

typedef enum
{
    BLUEGRAPH_MESSAGE_TYPE_TEXT,
    BLUEGRAPH_MESSAGE_TYPE_MEDIA,
    BLUEGRAPH_MESSAGE_TYPE_FILE
} MessageType;

typedef struct
{
    CapsuleType type;
    union
    {
        struct
        {
            bool isFinalChunk;
            MessageType messageType;
            char* msg;
            size_t msgLen;
        } send_message_info;
        struct
        {
            // Implement this
        } send_receipt_info;
        // Implement query
        struct
        {
            // Implement this
        } query_info;
        struct
        {
            // Implement this
        } query_reply_info;
    };
} Capsule_st;

typedef Capsule_st* Capsule;

Capsule createCapsule();
void freeCapsule(Capsule capsule);

cJSON* capsule2json(Capsule capsule);
Capsule json2capsule(cJSON* json);

#endif