#ifndef ERRORS
#define ERRORS

#include <stdio.h>
#include "colors.h"

typedef enum 
{
    ERR_SUCCESS = 0,
    ERR_NOT_FOUND,
    ERR_EOF_REACHED,
    ERR_NOT_A_NUMBER,
    ERR_OPENING_FILE,
    ERR_ALLOCATING_MEMORY,
    ERR_DATA,
    ERR_EMPTY_TREE,
    ERR_GRAPHVIZ_CONVERSION,
    ERR_IS_A_DUBLICATE
} err_code_e;

err_code_e process_error(err_code_e rc);

#endif /* ERRORS */
