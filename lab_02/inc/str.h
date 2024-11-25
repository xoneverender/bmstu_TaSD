#ifndef STR
#define STR

#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "errors.h"
#include "consts.h"

bool is_whole_number(long *num, char *buffer);
err_code read_string(FILE *f, char *buf, size_t len);
err_code validate_filename(char *filename);
void clear_input_buffer(void);
bool contains_word(const char *str, const char *word);

#endif /* STR */
