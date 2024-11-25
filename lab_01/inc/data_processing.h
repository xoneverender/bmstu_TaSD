#ifndef DATA_PROCESSING_H__
#define DATA_PROCESSING_H__

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "math_operations.h"
#include "validations.h"
#include "consts.h"
#include "structs.h"
#include "errors.h"

int parse_number(char *buffer, number_t *num, size_t MAX_SIZE);
void process_string(char *string);
void save_mantissa(uint8_t *mantissa, char *sign, uint8_t *len, char *string, int *dot_ind);
void save_exponent(int *exp, char *string);
int normalize_result(number_t *res);
char *remove_odd_zeros(char *string);
void zero_result(number_t *res);

#endif
