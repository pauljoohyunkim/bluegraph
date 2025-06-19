#include "../src/storage.h"

int main()
{
    char compressedAddress[13] = { 0 };
    char stringAddress[18] = "11:22:33:44:55:66";

    stringAddress2CompressedBDAddress(compressedAddress, stringAddress);

    return 0;
}