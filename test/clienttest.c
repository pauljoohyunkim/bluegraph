#include <string.h>
#include "../src/transaction.h"
#include "../src/conn.h"

int main()
{
    const char *serveraddress = "E8:48:B8:C8:20:00";
    Transaction transaction = createTransaction();

    transaction->type = BLUEGRAPH_TRANSACTION_TYPE_SEND_MESSAGE;
    transaction->send_message_info.sourceType = BLUEGRAPH_MESSAGE_SOURCE_BUFFER;
    transaction->send_message_info.source = "Hello World";
    transaction->send_message_info.messageLen = strlen(transaction->send_message_info.source);
    
    clientConnect(serveraddress, transaction);
    return 0;
}