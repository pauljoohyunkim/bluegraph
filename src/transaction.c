#include "capsule.h"
#include "transaction.h"

void serverTransaction()
{

}

// Given a socket 's' with a valid connection, this function handles the transaction.
void clientTransaction(Transaction transaction, int s)
{
    Capsule capsule = NULL;
    Packet packet = NULL;
    size_t packetSize = 0;

    if (!transaction) return;

    // From transaction, capsule and packets are built and sent as necessary.
    switch (transaction->type)
    {
        case BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE:
            switch (transaction->send_message_info.sourceType)
            {
                case BLUEGRAPH_MESSAGE_SOURCE_BUFFER:
                    // Request
                    capsule = createCapsule();
                    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST;
                    capsule->send_message_request_info.messageType = BLUEGRAPH_MESSAGE_TYPE_TEXT;
                    capsule->send_message_request_info.filenameLen = 0;
                    capsule->send_message_request_info.filename = NULL;
                    packet = capsule2packet(capsule, &packetSize);

                    // TODO: send packet and receive ack.

                    // Data
                    break;
                case BLUEGRAPH_MESSAGE_SOURCE_FILENAME:
                    break;
            }
            break;
        case BLUEGRAPH_TRANSACTION_TYPE_QUERY:
            break;
    }
}