#include "../src/storage.h"

int main()
{
    char compressedAddress[] = "001122334455";
    char stringAddress[18] = { 0 };

    compressedBAAddress2StringAddress(stringAddress, compressedAddress);

    return 0;
}