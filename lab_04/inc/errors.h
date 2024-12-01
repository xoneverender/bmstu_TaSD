#ifndef ERRORS
#define ERRORS

#include <stdio.h>
#include "colors.h"

typedef enum
{
    ERR_SUCCESS,
    ERR_EOF_REACHED,
    ERR_INVALID_STACK_INPUT,
    ERR_READING_STRING,
    ERR_ALLOCATING_MEMORY,
    ERR_NULL_PTR,
    ERR_EMPTY_STACK,
    ERR_EMPTY_STRING,
    ERR_STACK_OVERFLOW,
    ERR_CALCULATIONS,
    ERR_DIVISION_BY_ZERO,
    ERR_INVALID_EXPRESSION_LEN,
    ERR_OPENING_FILE,
    ERR_INVALID_DATA_IN_FILE
} err_code_e;

err_code_e process_error(err_code_e rc);

#endif /* ERRORS */
