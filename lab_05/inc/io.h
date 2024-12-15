#ifndef IO
#define IO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
#include "simulation.h"
#include "str.h"
#include "compare.h"

typedef enum 
{
    EXIT = 0,
    COMPARE = 1, 
    SIMULATE = 2
} action_e;

void main_menu(void);
void horizontal_rule(void);
void input_arrow(void);
err_code_e get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message);
err_code_e get_double_from_stdin(char *prompt, double *num, double min_val, char *error_message);
err_code_e process_action(action_e action);

#endif /* IO */
