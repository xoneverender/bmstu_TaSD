#ifndef USER_INTERFACE
#define USER_INTERFACE

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include "files.h"
#include "errors.h"
#include "consts.h"
#include "colors.h"

void show_welcoming_message(void);
void add_horizontal_rule(void);
void show_menu(void);
void add_input_arrow(void);
void get_users_choice(long *choice);
void print_book_arr(const book_t *records, const int records_num);
void print_book(const book_t record);
void print_key_arr(const key_t *keys, const int keys_num);
void print_table_using_keys(const book_t *records, const key_t *keys, const int records_num);
err_code confirm_file_retry(void);
err_code ask_to_proceed(void);
err_code get_file(char *filename, book_t *records, key_t *keys, int *records_num);

#endif /* USER_INTERFACE */
