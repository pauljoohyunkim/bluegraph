#include <endian.h>
#include <stdlib.h>
#include <stdint.h>
#include "capsule.h"

Capsule createCapsule()
{
    Capsule capsule = (Capsule) calloc(1, sizeof(Capsule_st));

    return capsule;
}

void freeCapsule(Capsule capsule)
{
    switch (capsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
            free(capsule->send_message_request_info.filename);
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            free(capsule->send_message_data_info.msg);
            break;
    }
    free(capsule);
}

Packet capsule2packet(Capsule capsule, size_t *pSize)
{
    size_t index = 0;
    Packet packet = NULL;

    if (!capsule) return NULL;

    // Precompute the length of the packet.
    index += sizeof(uint8_t);       // type
    switch (capsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
            index += sizeof(uint8_t);       // messageType
            index += sizeof(uint64_t);      // msgLen
            index += sizeof(uint8_t);       // filenameLen
            index += capsule->send_message_request_info.filenameLen;    // filename
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            index += sizeof(uint8_t);       // isFinalChunk
            index += sizeof(uint64_t);      // msgLen
            index += capsule->send_message_data_info.msgLen;    // msg
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK:
            index += sizeof(uint8_t);       // ack
            break;
        case BLUEGRAPH_CAPSULE_TYPE_QUERY:
        case BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY:
            break;
    }
    *pSize = index;

    // Start building packet.
    index = 0;
    packet = (Packet) calloc(*pSize, sizeof(uint8_t));
    packet[index] = (uint8_t) capsule->type;
    index += sizeof(uint8_t);
    switch (capsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
            packet[index] = (uint8_t) capsule->send_message_request_info.messageType;
            index += sizeof(uint8_t);
            *((uint64_t*) (packet + index)) = (uint64_t) htobe64(capsule->send_message_request_info.msgLen);
            index += sizeof(uint64_t);
            packet[index] = capsule->send_message_request_info.filenameLen;
            index += sizeof(uint8_t);
            strncpy(packet + index, capsule->send_message_request_info.filename, capsule->send_message_request_info.filenameLen);
            index += capsule->send_message_request_info.filenameLen;
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            packet[index] = (uint8_t) capsule->send_message_data_info.isFinalChunk;
            index += sizeof(uint8_t);
            *((uint64_t*) (packet + index)) = (uint64_t) htobe64(capsule->send_message_request_info.msgLen);
            index += sizeof(uint64_t);
            strncpy(packet + index, capsule->send_message_data_info.msg, capsule->send_message_data_info.msgLen);
            index += capsule->send_message_data_info.msgLen;
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK:
            packet[index] = (uint8_t) capsule->send_message_request_ack_info.ack;
            index += sizeof(uint8_t);
            break;
        case BLUEGRAPH_CAPSULE_TYPE_QUERY:
        case BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY:
            break;
    }

    return packet;
}

Capsule packet2capsule(Packet packet, size_t size)
{
    Capsule capsule = calloc(1, sizeof(Capsule_st));
    size_t index = 0;
    uint8_t c = 0;
    if (!capsule) return NULL;

    capsule->type = (CapsuleType) packet[index];
    index += sizeof(uint8_t);
    switch (capsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST:
            capsule->send_message_request_info.messageType = (MessageType) packet[index];
            index += sizeof(uint8_t);
            capsule->send_message_request_info.msgLen = (uint64_t) be64toh(*((uint64_t*) (packet + index)));
            index += sizeof(uint64_t);
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            capsule->send_message_data_info.isFinalChunk = (bool) packet[index];
            index += sizeof(uint8_t);
            capsule->send_message_data_info.msgLen = (uint64_t) be64toh(*((uint64_t*) (packet + index)));
            index += sizeof(uint64_t);
            capsule->send_message_data_info.msg = calloc(capsule->send_message_data_info.msgLen, sizeof(uint8_t));
            memcpy(capsule->send_message_data_info.msg, packet + index, capsule->send_message_data_info.msgLen);
            index += capsule->send_message_data_info.msgLen;
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST_ACK:
            capsule->send_message_request_ack_info.ack = (bool) packet[index];
            index += sizeof(uint8_t);
            break;
        case BLUEGRAPH_CAPSULE_TYPE_QUERY:
            break;
        case BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY:
            break;
    }

    return capsule;
}