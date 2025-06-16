#include "transaction.h"

void serverTransaction()
{

}

void clientTransaction(Transaction transaction)
{
    if (!transaction) return;

    switch (transaction->type)
    {
        case BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE:
            break;
        case BLUEGRAPH_TRANSACTION_TYPE_QUERY:
            break;
    }
}