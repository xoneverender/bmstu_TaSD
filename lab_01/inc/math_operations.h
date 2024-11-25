#ifndef MATH_OPERATIONS_H__
#define MATH_OPERATIONS_H__

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "structs.h" 
#include <stdio.h>

void product(number_t *num1, number_t *num2, number_t *res);
void rounding(number_t *num);
int count_digits_num(char *string);
int count_el_in_string(char *string, char el);

#endif
