#ifndef IO
#define IO

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "errors.h"
#include "str.h"
#include "colors.h"

void main_menu(void);
void horizontal_rule(void);
void input_arrow(void);
err_code_e get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message);
err_code_e get_double_from_stdin(char *prompt, double *num, double min_val, double max_val, char *error_message);
err_code_e get_string_from_stdin(char *prompt, char **string, char *error_message);

#endif /* IO */
