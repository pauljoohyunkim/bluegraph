#include <stdio.h>
#include <stdint.h>
#include <time.h>

int main()
{
    time_t epoch;
    uint64_t epochnum;
    char epochstr[21] = { 0 };
    
    epoch = time(NULL);
    epochnum = (uint64_t) epoch;
    snprintf(epochstr, sizeof(epochstr), "%llu", epochnum);
    //printf("Current time: %ld\n", epochnum);
}