#ifndef MENU_ACTIONS
#define MENU_ACTIONS

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "errors.h"
#include "matrix.h"

err_code operate_csr_addition(const csr_matrix_t csr_1, const csr_matrix_t csr_2, csr_matrix_t *csr_result);
err_code operate_standart_addition(const matrix_t standart_1, const matrix_t standart_2, matrix_t *result);
err_code operate_algorithms_compare(void);
err_code operate_matrix_reading(matrix_t *matrix, csr_matrix_t *csr_matrix);
err_code operate_matrix_output(const matrix_t matrix, const csr_matrix_t csr_matrix);

#include "io.h"

#endif /* MENU_ACTIONS */
