#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include "capsule.h"
#include "conn.h"
#include "transaction.h"
#include "storage.h"

Transaction createTransaction()
{
    Transaction transaction = calloc(1, sizeof(Transaction_st));

    return transaction;
}

// Given a socket 's' with a valid connection, this function handles the transaction.
// Returns -1 on error
int serverTransaction(int s, BluegraphStorage storage)
{
    struct sockaddr_rc rem_addr = { 0 };
    int status = 0;
    int client = 0;
    uint8_t buf[BLUEGRAPH_CHUNK_SIZE] = { 0 };
    char bdaddr[18] = { 0 };
    uint8_t compressedBDAddress[13] = { 0 };
    char *bdaddr_dir = NULL;
    Capsule clientCapsule = NULL;
    Capsule serverCapsule = NULL;
    Packet serverPacket = NULL;
    size_t serverPacketSize = 0;
    MessageFileInfo messageFileInfo = NULL;
    int opt = sizeof(rem_addr);

    client = accept(s, (struct sockaddr *)&rem_addr, &opt);
    ba2str(&rem_addr.rc_bdaddr, bdaddr);
    stringAddress2CompressedBDAddress(compressedBDAddress, bdaddr);
    bdaddr_dir = calloc(strlen(storage->dir) + 2 + sizeof(compressedBDAddress), sizeof(char));
    strcpy(bdaddr_dir, storage->dir);
    strcat(bdaddr_dir, "/");
    strcat(bdaddr_dir, compressedBDAddress);
    fprintf(stderr, "accepted connection from %s\n", bdaddr);
    memset(buf, 0, sizeof(buf));

    status = read(client, buf, BLUEGRAPH_CHUNK_SIZE);
    if (status <= 0)
    {
        free(bdaddr_dir);
        close(client);
        return -1;
    }
    clientCapsule = packet2capsule(buf, status);
    if (!clientCapsule)
    {
        free(bdaddr_dir);
        close(client);
        return -1;
    }

    switch (clientCapsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
            // TODO: For now, send positive ack for all messages.
            // TODO: For now, only handle text message.
            messageFileInfo = calloc(1, sizeof(MessageFileInfo_st));
            messageFileInfo->direction = BLUEGRAPH_INCOMING;
            messageFileInfo->isText = clientCapsule->send_message_request_info.messageType == BLUEGRAPH_MESSAGE_TYPE_TEXT;
            messageFileInfo->info = calloc(clientCapsule->send_message_request_info.msgLen, sizeof(uint8_t));
            messageFileInfo->time = time(NULL);

            serverCapsule = createCapsule();
            serverCapsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK;
            serverCapsule->send_message_request_ack_info.ack = true;
            serverPacket = capsule2packet(serverCapsule, &serverPacketSize);
            freeCapsule(serverCapsule);

            status = write(client, serverPacket, serverPacketSize);
            free(serverPacket);
            if (status < 0)
            {
                fprintf(stderr, "Could not send ack to client.\n");
                break;
            }

            // TODO: Getting packet from client
            status = read(client, buf, BLUEGRAPH_CHUNK_SIZE);
            if (status < 0)
            {
                fprintf(stderr, "Could not get packet from client.\n");
                break;
            }
            clientCapsule = packet2capsule(buf, status);
            if (!clientCapsule)
            {
                fprintf(stderr, "Could not create packet from client.\n");
                freeCapsule(clientCapsule);
                break;
            }
            memcpy(messageFileInfo->info, clientCapsule->send_message_data_info.msg, clientCapsule->send_message_request_info.msgLen);
            writeMessageInfo(messageFileInfo, bdaddr_dir);
            dumpMessageInfo(messageFileInfo, bdaddr);
            freeCapsule(clientCapsule);
            freeMessageInfo(messageFileInfo);
            break;
        default:
            break;
    }
    free(bdaddr_dir);
    close(client);
    return status;
}

// Given a socket 's' with a valid connection, this function handles the transaction.
// Returns -1 on error
int clientTransaction(Transaction transaction, int s)
{
    Capsule capsule = NULL;
    Packet packet = NULL;
    size_t packetSize = 0;
    int status = 0;
    uint8_t buf[BLUEGRAPH_CHUNK_SIZE] = { 0 };

    if (!transaction) return -1;

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
                        fprintf(stderr, "Error sending message request.\n");
                        return -1;
                    }
                    // Receive ack
                    status = read(s, buf, BLUEGRAPH_CHUNK_SIZE);
                    if (status < 0)
                    {
                        fprintf(stderr, "Could not get packet from the receipient.\n");
                        return -1;
                    }
                    packetSize = status;
                    capsule = packet2capsule(buf, packetSize);
                    if (!capsule)
                    {
                        fprintf(stderr, "Could not construct capsule. Is it a valid packet?\n");
                        return -1;
                    }
                    if (capsule->type != BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK)
                    {
                        freeCapsule(capsule);
                        fprintf(stderr, "Expected an ACK packet.\n");
                        return -1;
                    }
                    if (capsule->send_message_request_ack_info.ack == false)
                    {
                        freeCapsule(capsule);
                        fprintf(stderr, "The recipient blocked the message.\n");
                        return -1;
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
                    if (status < 0)
                    {
                        fprintf(stderr, "Failed to send data.\n");
                        return -1;
                    }
                    free(packet);
                    
                    break;
                case BLUEGRAPH_MESSAGE_SOURCE_FILENAME:
                    break;
            }
            break;
        case BLUEGRAPH_TRANSACTION_TYPE_QUERY:
            break;
    }

    return 0;
}