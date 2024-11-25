#ifndef ERRORS_H__
#define ERRORS_H__

#include "stdlib.h"
#include "stdio.h"

#define ERR_SUCCESS 0
#define ERR_EMPTY_LINE 1
#define ERR_INCORRECT_STR 2
#define ERR_MANTISSA_TOO_LONG 3
#define ERR_EXP_TOO_LONG 4
#define ERR_NOT_A_REAL_NUM 5
#define ERR_NOT_A_WHOLE_NUM 6
#define ERR_EXP_OVERFLOW 7

void process_error(size_t err_code);

#endif
