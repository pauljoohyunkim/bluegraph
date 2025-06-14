#ifndef __CAPSULE_H__
#define __CAPSULE_H__

#include <stdlib.h>
#include <string.h>

typedef enum
{
    BLUEGRAPH_CAPUSLE_TYPE_SEND_MESSAGE,    // Sending message
    BLUEGRAPH_CAPSULE_TYPE_SEND_RECEIPT,    // Acknowledgment of receiving message
    BLUEGRAPH_CAPSULE_QUERY                 // Querying if node can reach destination.
} CapsuleType;

typedef struct
{
    CapsuleType type;
    union
    {
        struct
        {
            char* msg;
            size_t msglen;
        } send_message_info;
        struct
        {
            // Implement this
        } send_receipt_info;
        // Implement query
    };
} Capsule_st;


#endif