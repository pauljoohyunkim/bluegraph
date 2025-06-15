#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "../src/capsule.h"

void sendMessageRequestInfo()
{
    Capsule capsule = createCapsule();
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_REQUEST;
    char data[] = "Hello World";
    capsule->send_message_request_info.msgLen = strlen(data);

    cJSON* json = capsule2json(capsule);
    char *jsonstr = cJSON_Print(json);
    printf("%s\n", jsonstr);
    cJSON_free(json);
    freeCapsule(capsule);

    // Reading back
    json = cJSON_Parse(jsonstr);
    capsule = json2capsule(json);
    freeCapsule(capsule);
    cJSON_free(jsonstr);
}

void sendMessageDataInfo()
{
    Capsule capsule = createCapsule();
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE_DATA;
    capsule->send_message_data_info.isFinalChunk = true;
    capsule->send_message_data_info.msg = "Hello World";

    cJSON* json = capsule2json(capsule);
    char *jsonstr = cJSON_Print(json);
    printf("%s\n", jsonstr);
    cJSON_free(json);
    cJSON_free(jsonstr);
}

int main()
{
    sendMessageRequestInfo();
    //sendMessageDataInfo();
}