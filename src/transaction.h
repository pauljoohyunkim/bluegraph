#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

#include <stdint.h>
#include <stddef.h>

/*
For BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE,
    1. Client sends SendMessageRequest.
    2. Server receives the request, recognizes that it is BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE, and sends a response SendMessageRequestAck
    3. Client repeatedly sends message over multiple chunks until all data is sent.
*/

typedef enum
{
    BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE,
    BLUEGRAPH_TRANSACTION_TYPE_QUERY
} TransactionType;

typedef enum
{
    BLUEGRAPH_MESSAGE_SOURCE_BUFFER,    // Text Message; filename is set to NULL.
    BLUEGRAPH_MESSAGE_SOURCE_FILENAME
} MessageSourceType;

typedef struct
{
    TransactionType type;
    union
    {
        struct
        {
            MessageSourceType sourceType;
            uint8_t *source;
            size_t messageLen;
        } send_message_info;
    };
} Transaction_st;

typedef Transaction_st *Transaction;

Transaction createTransaction();
void serverTransaction();
int clientTransaction(Transaction transaction, int s);

#endif