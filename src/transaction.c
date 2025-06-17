#include "capsule.h"
#include"conn.h"
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
    int status = 0;
    uint8_t buf[BLUEGRAPH_CHUNK_SIZE] = { 0 };

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
                    freeCapsule(capsule);
                    capsule = NULL;

                    // Send Packet
                    status = write(s, packet, packetSize);
                    free(packet);
                    packet = NULL;
                    if (status < 0)
                    {
                        // TODO: More sophisticated quit and handle error.
                        return;
                    }
                    // TODO: receive ack.
                    status = read(s, buf, BLUEGRAPH_CHUNK_SIZE);
                    if (status < 0)
                    {
                        // TODO: Handle error when receving ack.
                        return;
                    }
                    packetSize = status;
                    capsule = packet2capsule(buf, packetSize);
                    if (!capsule)
                    {
                        // TODO: Handle error
                    }
                    if (capsule->type != BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK)
                    {
                        // TODO: Handle error
                    }
                    if (capsule->send_message_request_ack_info.ack == false)
                    {
                        // TODO: Server blocks the request.
                    }
                    freeCapsule(capsule);
                    capsule = NULL;

                    // Data
                    // TODO: Divide the message into chunks.
                    capsule = createCapsule();
                    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA;
                    capsule->send_message_data_info.isFinalChunk = true;
                    capsule->send_message_data_info.msgLen = transaction->send_message_info.messageLen;
                    capsule->send_message_data_info.msg = calloc(transaction->send_message_info.messageLen, sizeof(uint8_t));
                    memcpy(capsule->send_message_data_info.msg, transaction->send_message_info.source, transaction->send_message_info.messageLen);
                    
                    packet = capsule2packet(capsule, &packetSize);
                    free(capsule);
                    capsule = NULL;
                    status = write(s, packet, packetSize);
                    free(packet);
                    
                    break;
                case BLUEGRAPH_MESSAGE_SOURCE_FILENAME:
                    break;
            }
            break;
        case BLUEGRAPH_TRANSACTION_TYPE_QUERY:
            break;
    }
}