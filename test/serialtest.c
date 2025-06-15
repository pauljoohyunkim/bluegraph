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

    Packet packet = capsule2packet(capsule, &packet_length);
    
    free(packet);
    freeCapsule(capsule);
}

void sendMessageDataInfo()
{
    Capsule capsule = createCapsule();
    size_t packet_length = 0;
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA;
    capsule->send_message_data_info.isFinalChunk = true;
    capsule->send_message_data_info.msg = "Hello World";
    capsule->send_message_data_info.msgLen = strlen(capsule->send_message_data_info.msg);

    Packet packet = capsule2packet(capsule, &packet_length);
    free(packet);
    freeCapsule(capsule);
}

int main()
{
    sendMessageRequestInfo();
    sendMessageDataInfo();
}