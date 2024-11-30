#define _GNU_SOURCE
#ifndef STR
#define STR

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "errors.h"
#include "stack.h"

#define STRING_LEN 100

err_code_e read_string(FILE *stream, char **buf, size_t *len);
bool is_long_int(long *num, char *buffer);
bool is_float(float *num, char *buffer);
err_code_e extract_token(char **token, char *start, char *end);
err_code_e parse_string(char *buf, void *stack, err_code_e (*push_func)(void *, char *));

#endif /* STR */
