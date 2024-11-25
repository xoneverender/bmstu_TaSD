#ifndef UTILS_H__
#define UTILS_H__

#include <stdio.h>
#include "errors.h"
#include "str.h"
#include "structs.h"
#include "user_interface.h"

err_code open_file(char *filename);
err_code read_records_num(FILE *f, int *records_num);
err_code read_file(char *filename, book_t *records, key_t *keys, int *records_num);
err_code save_in_file(book_t *records, int records_num, char *filename);

#endif
