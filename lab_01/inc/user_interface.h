#ifndef USER_INTERFACE
#define USER_INTERFACE

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "consts.h"
#include "structs.h"
#include "data_processing.h"
#include "errors.h"

void show_program_purpose(void);
int read_line(char *buffer, size_t buffer_size, size_t MAX_SIZE);
void print_result(number_t *res);

#endif /* USER_INTERFACE */
