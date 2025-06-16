#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum
{
    BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST,        // Request to send message.
    BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA,           // Data of message.
    BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK,    // Acknowledgement of request to send message.
    BLUEGRAPH_CAPSULE_TYPE_QUERY,                       // Querying if node can reach destination.
    BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY                  // Reply to the query
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
            MessageType messageType;
            uint64_t msgLen;
            uint8_t filenameLen;
            uint8_t *filename;
        } send_message_request_info;
        struct
        {
            bool isFinalChunk;
            uint64_t msgLen;
            uint8_t *msg;
        } send_message_data_info;
        struct
        {
            // Implement this
            bool ack;
        } send_message_request_ack_info;
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

typedef Capsule_st *Capsule;
typedef uint8_t *Packet;

Capsule createCapsule();
void freeCapsule(Capsule capsule);

Packet capsule2packet(Capsule capsule, size_t *pSize);
Capsule packet2capsule(Packet packet, size_t size);

#endif