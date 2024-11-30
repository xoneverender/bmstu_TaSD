#ifndef IO
#define IO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "colors.h"
#include "stack.h"
#include "errors.h"
#include "menu_actions.h"

typedef enum 
{
    MAIN_MENU, 
    STACK_OPERATIONS_MENU
} menu_e;

typedef enum
{
    EXIT,
    ADD_ELEM,
    REMOVE_ELEM,
    CALCULATE,
    SHOW_STACK,
    SHOW_REMOWED,
    BACK_TO_MAIN_MENU
} action_e;

void horizontal_rule(void);
void input_arrow(void);
void main_menu(void);
void stack_operations_menu(void);
err_code_e get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message);
err_code_e process_actions(const action_e action, menu_e *menu, removed_t *removed, action_funcs_t *funcs);

#endif /* IO */
