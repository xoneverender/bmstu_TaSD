#ifndef MATRIX
#define MATRIX

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include "errors.h"

typedef struct
{
    double *N; // Ненулевые значения матрицы
    int *J; // Индексы столбцов этих элементов
    int *I; // Индексы в массиве N первых элементов каждой строки исходной матрицы
    size_t rows;
    size_t columns;
} csr_matrix_t;

typedef struct 
{
    size_t rows;
    size_t columns;
    double **ptrs;
} matrix_t;

err_code standart_to_csr(const matrix_t standart, csr_matrix_t *csr);
err_code allocate_memory_csr(csr_matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzeros);
err_code allocate_memory_standart(matrix_t *matrix, const size_t rows, const size_t columns);
err_code standart_addition(const matrix_t matrix_1, const matrix_t matrix_2, matrix_t *result);
err_code csr_addition(const csr_matrix_t A, const csr_matrix_t B, csr_matrix_t *C);
err_code csr_to_standart(const csr_matrix_t csr, matrix_t *standart);
unsigned long long count_standart_addition_time(const matrix_t matrix_1, const matrix_t matrix_2, matrix_t *result);
unsigned long long count_csr_addition_time(const csr_matrix_t matrix_1, const csr_matrix_t matrix_2, csr_matrix_t *result);
void init_standart(matrix_t *matrix, size_t nonzeros);
void free_standart(matrix_t *matrix);
void free_csr(csr_matrix_t *matrix);
void print_standart(const matrix_t matrix);
void print_csr(const csr_matrix_t matrix);
void shuffle(double *arr, const int elems_num);
void set_matrix_to_zero(matrix_t *matrix);

#endif /* MATRIX */
