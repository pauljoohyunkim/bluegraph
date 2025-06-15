#include <stdlib.h>
#include <cjson/cJSON.h>
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

cJSON* capsule2json(Capsule capsule)
{
    if (!capsule) return NULL;

    cJSON *json = cJSON_CreateObject();
    
    switch (capsule->type)
    {
        case BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE:
            cJSON_AddBoolToObject(json, "isFinalChunk", capsule->send_message_info.isFinalChunk);
            switch (capsule->send_message_info.messageType)
            {
                case BLUEGRAPH_MESSAGE_TYPE_TEXT:
                    cJSON_AddStringToObject(json, "msgType", "text");
                    break;
                case BLUEGRAPH_MESSAGE_TYPE_MEDIA:
                    cJSON_AddStringToObject(json, "msgType", "media");
                    break;
                case BLUEGRAPH_MESSAGE_TYPE_FILE:
                    cJSON_AddStringToObject(json, "msgType", "file");
                    break;
            }
            cJSON_AddStringToObject(json, "msg", capsule->send_message_info.msg);
            cJSON_AddNumberToObject(json, "msgLen", capsule->send_message_info.msgLen);
            break;
        default:
            break;
    }

    return json;
}

Capsule json2capsule(cJSON* json)
{
    return NULL;
}