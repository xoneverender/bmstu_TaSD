#ifndef MENU_ACTIONS
#define MENU_ACTIONS

#include <stdio.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include "errors.h"
#include "files.h"
#include "consts.h"
#include "files.h"
#include "structs.h"
#include "user_interface.h"

err_code process_choice(long choice, char *filename, book_t *records, key_t *keys, int *records_num);
err_code add_record(book_t *records, int *records_num);
err_code delete_record(book_t *records, int *records_num);
err_code find_record(book_t *records, int records_num);
unsigned long long count_sorting_time(void *base, size_t elems_num, size_t elems_size, int (*compare)(const void *, const void *), void (*sort)(void *, size_t, size_t, int (*)(const void *, const void *)));
void compare_sorts(book_t *records, key_t *keys, int records_num);

#endif /* MENU_ACTIONS */
