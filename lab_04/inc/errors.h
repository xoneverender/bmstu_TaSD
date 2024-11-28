#ifndef ERRORS
#define ERRORS

#include <stdio.h>

typedef enum
{
    ERR_SUCCESS,
    ERR_EOF_REACHED,
    ERROR_INVALID_STACK_INPUT,
    ERR_INVALID_INPUT,
    ERR_ALLOCATING_MEMORY
} err_code_e;

#endif /* ERRORS */
