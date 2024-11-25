#ifndef IO
#define IO

#include <stdio.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>
#include "errors.h"
#include "files.h"
#include "matrix.h"
#include "str.h"
#include "limits.h"
#include "colors.h"
#include "menu_actions.h"

typedef enum 
{
    EXIT,
    SUM_STANDART,
    SUM_CSR,
    COMPARE_ALGS,
    VIEW_MATRICES,
    READ_FIRST_MATRIX,
    READ_SECOND_MATRIX
} action_e;

typedef enum
{
    STANDART = 1,
    CSR = 2
} output_format_e;

typedef enum 
{
    STANDARD = 1,
    COORDINATE = 2
} input_format_e;

typedef enum 
{
    TXTFILE = 1,
    STDIN = 2
} source_e;

void main_menu(void);
void first_iteration_menu(void);
void horizontal_rule(void);
void input_arrow(void);
err_code get_matrix_source(source_e *source);
err_code get_output_format(output_format_e *format);
err_code read_matrix_standart(matrix_t *matrix, const long rows, const long columns);
err_code read_matrix_coordinate(matrix_t *matrix, const long rows, const long columns);
err_code get_input_format(input_format_e *format);
err_code get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message);
err_code get_double_from_stdin(char *prompt, double *num, char *error_message, bool nonzero);
err_code get_filename(char **filename);
err_code get_matrix(matrix_t *matrix);
err_code read_matrix_from_stdin(matrix_t *matrix);
err_code process_actions(const action_e action, matrix_t *standart_1, matrix_t *standart_2, csr_matrix_t *csr_1, csr_matrix_t *csr_2);

#endif /* IO */
