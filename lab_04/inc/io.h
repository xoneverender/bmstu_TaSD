#ifndef IO
#define IO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "colors.h"
#include "stack.h"
#include "errors.h"

typedef enum
{
    ADD_ELEM,
    REMOVE_ELEM,
    CALCULATE,
    SHOW_STACK,
    SHOW_REMOWED,
    BACK_TO_MAIN_MENU,
    EXIT
} action_e;

void horizontal_rule(void);
void input_arrow(void);
void main_menu(void);
void stack_operations_menu(void);

#endif /* IO */
