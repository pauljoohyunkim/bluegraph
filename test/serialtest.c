#include <stdio.h>
#include <string.h>
#include <cjson/cJSON.h>
#include "../src/capsule.h"

int main()
{
    Capsule capsule = createCapsule();
    capsule->type = BLUEGRAPH_CAPSULE_TYPE_SEND_MESSAGE;
    capsule->send_message_info.msg = "Hello World";
    capsule->send_message_info.msgLen = strlen(capsule->send_message_info.msg);

    cJSON* json = capsule2json(capsule);
    char *jsonstr = cJSON_Print(json);
    printf("%s\n", jsonstr);

}