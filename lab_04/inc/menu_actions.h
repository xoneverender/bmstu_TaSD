#ifndef MENU_ACTIONS
#define MENU_ACTIONS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "io.h"
#include "stack.h"
#include "str.h"
#include "errors.h"
#include "calculations.h"
#include "files.h"

err_code_e operate_adding_element(void *stack, err_code_e (*push_func)(void*, data_t*));
err_code_e operate_removing_element(void *stack, removed_t *removed, err_code_e (*pop_func)(void *, data_t **el));
void operate_printing_array_stack(void *top);
void operate_printing_list_stack(void *top);
void operate_printing_removed(removed_t *removed);
err_code_e operate_calculating_rpn(void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode, bool verbose);
err_code_e operate_calculating_rpn_from_string(err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode);
err_code_e operate_comparizon(void);

#endif /* MENU_ACTIONS */
