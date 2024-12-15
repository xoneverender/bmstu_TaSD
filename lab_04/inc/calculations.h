#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#define _GNU_SOURCE
#include <x86intrin.h>

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include "errors.h"
#include "str.h"
#include "stack.h"

err_code_e rpn(void *stack_a, void *stack_b, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *));
err_code_e calculate(void *stack_a, void *stack_b, char *skipped_signs, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *));
unsigned long long count_calculations_time(char *string, action_funcs_t *funcs, mode_e mode);
err_code_e calculate_rpn_from_string(char *buf, void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode, bool verbose);
err_code_e rpn_by_mode(void *stack_a, void *stack_b, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode);

#endif /* CALCULATIONS_H */
