#ifndef CALCULATIONS
#define CALCULATIONS

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "errors.h"
#include "str.h"
#include "stack.h"

err_code_e rpn(void *stack_a, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *));
err_code_e calculate(void *stack_a, void *stack_b, char *skipped_signs, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *));

#endif /* CALCULATIONS */
