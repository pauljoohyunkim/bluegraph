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
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            // TODO: For files, the filename should also be included.
            index += sizeof(uint8_t);       // isFinalChunk
            index += sizeof(uint64_t);      // msgLen
            index += capsule->send_message_data_info.msgLen;    // msg
        case BLUEGRAPH_CAPSULE_TYPE_SEND_RECEIPT:
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
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA:
            packet[index] = (uint8_t) capsule->send_message_data_info.isFinalChunk;
            index += sizeof(uint8_t);
            *((uint64_t*) (packet + index)) = (uint64_t) htobe64(capsule->send_message_request_info.msgLen);
            index += sizeof(uint64_t);
            strncpy(packet + index, capsule->send_message_data_info.msg, capsule->send_message_data_info.msgLen);
            index += capsule->send_message_data_info.msgLen;
            break;
        case BLUEGRAPH_CAPSULE_TYPE_SEND_RECEIPT:
        case BLUEGRAPH_CAPSULE_TYPE_QUERY:
        case BLUEGRAPH_CAPSULE_TYPE_QUERY_REPLY:
            break;
    }

    return packet;
}

Capsule packet2capsule(Packet packet, size_t size)
{

}