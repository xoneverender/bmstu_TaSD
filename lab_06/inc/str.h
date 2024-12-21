#define _GNU_SOURCE
#ifndef STR
#define STR

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "errors.h"

err_code_e read_string(FILE *f, char **buffer);
bool is_long_int(long *num, char *buffer);
bool is_double(double *num, char *buffer);

#endif /* STR */
