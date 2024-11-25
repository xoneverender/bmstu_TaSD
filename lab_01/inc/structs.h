#ifndef STRUCTS__
#define STRUCTS__

#include <stdint.h>
#include "consts.h"

typedef struct
{
    char sign;
    uint8_t mantissa[MAX_RES_SIZE];
    uint8_t len;
    int exp;
} number_t;

#endif
