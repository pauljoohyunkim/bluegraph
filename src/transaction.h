#ifndef __TRANSACTION_H__
#define __TRANSACTION_H__

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
    BLUEGRAPH_MESSAGE_SOURCE_BUFFER,
    BLUEGRAPH_MESSAGE_SOURCE_FILE
} MessageSourceType;

typedef struct
{
    TransactionType type;
    union
    {
        struct
        {
            MessageSourceType sourceType;
            void *source;
        } send_message_info;
    };
} Transaction_st;

typedef Transaction_st *Transaction;

#endif