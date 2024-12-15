#ifndef ERRORS_H
#define ERRORS_H

#include <stdio.h>
#include "colors.h"

typedef enum
{
    ERR_SUCCESS,
    ERR_EOF_REACHED,
    ERR_READING_STRING,
    ERR_ALLOCATING_MEMORY,
    ERR_NULL_PTR,
    ERR_EMPTY_QUEUE,
    ERR_EMPTY_STRING,
    ERR_QUEUE_OVERFLOW,
} err_code_e;

err_code_e process_error(err_code_e rc);

#endif /* ERRORS_H */
