#include <stdlib.h>
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