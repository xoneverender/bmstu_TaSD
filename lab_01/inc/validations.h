#ifndef VALIDATIONS_H__
#define VALIDATIONS_H__

#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include "errors.h"
#include "consts.h"
#include "data_processing.h"

int validate_mantissa(char *string, size_t MAX_SIZE);
int validate_exponent(char *string);
bool is_real_number(char *string);
bool is_whole_number(char *string);
bool is_null(number_t *num);

#endif
