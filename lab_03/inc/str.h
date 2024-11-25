#define _GNU_SOURCE
#ifndef STR
#define STR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "errors.h"

#define STRING_LEN 100

bool is_whole_number(long *num, char *buffer);
err_code read_string(FILE *stream, char **buf, size_t *len);
bool is_double(double *num, char *buffer);

#endif /* STR */