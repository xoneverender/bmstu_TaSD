#ifndef ERRORS
#define ERRORS

#include <stdio.h>
#include "colors.h"

typedef enum
{
    ERR_SUCCESS,
    ERR_INVALID_INPUT,
    ERR_EOF_REACHED,
    ERR_OPENING_FILE,
    ERR_EMPTY_FILE,
    ERR_INVALID_FILE_FORMAT,
    ERR_ALLOCATING_MEMORY,
    ERR_MTX_DIM_MISMATCH,
} err_code;

err_code process_error(err_code error);

#endif /* ERRORS */
