#ifndef ERRORS_H__
#define ERRORS_H__

#include <stdio.h>
#include "consts.h"

typedef enum
{
    ERR_SUCCESS,
    ERR_OPENING_FILE,
    ERR_TOO_LONG_FILENAME,
    ERR_FILENAME_NOT_PROVIDED,
    ERR_INVALID_RECORDS_NUM,
    ERR_EMPTY_FILE,
    ERR_INVALID_FILE_FORMAT,
    ERR_ADDING_RECORD,
    ERR_DELITING_RECORD,
    ERR_NOT_DELETED,
    ERR_NOT_FOUND,
    ERR_STRING_TOO_LONG,
    ERR_EMPTY_STRING
} err_code;

#include "str.h"

void process_error(err_code error_code);

#endif
