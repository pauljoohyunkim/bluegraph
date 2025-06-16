#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../src/capsule.h"

void sendMessageRequestInfo()
{
    Capsule capsule = createCapsule();
    size_t packet_length = 0;
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST;
    capsule->send_message_request_info.messageType = BLUEGRAPH_MESSAGE_TYPE_FILE;
    char data[] = "Hello World";
    capsule->send_message_request_info.msgLen = strlen(data);
    capsule->send_message_request_info.filenameLen = 5;
    capsule->send_message_request_info.filename = calloc(100, sizeof(uint8_t));
    strcpy(capsule->send_message_request_info.filename, "a.txt");

    Packet packet = capsule2packet(capsule, &packet_length);
    
    freeCapsule(capsule);

    capsule = packet2capsule(packet, packet_length);
    freeCapsule(capsule);
    free(packet);
}

void sendMessageDataInfo()
{
    Capsule capsule = createCapsule();
    size_t packet_length = 0;
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA;
    capsule->send_message_data_info.isFinalChunk = true;
    capsule->send_message_data_info.msg = calloc(100, sizeof(uint8_t));
    strcpy(capsule->send_message_data_info.msg, "Hello World");
    capsule->send_message_data_info.msgLen = strlen(capsule->send_message_data_info.msg);

    Packet packet = capsule2packet(capsule, &packet_length);

    freeCapsule(capsule);

    capsule = packet2capsule(packet, packet_length);
    freeCapsule(capsule);
    free(packet);
}

int main()
{
    sendMessageRequestInfo();
    sendMessageDataInfo();
}