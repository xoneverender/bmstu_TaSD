#include "matrix.h"

void free_standart(matrix_t *matrix)
{
    if (matrix == NULL || matrix->ptrs == NULL)
        return;

    free(matrix->ptrs[0]);
    free(matrix->ptrs);
}

void free_csr(csr_matrix_t *matrix)
{
    if (matrix == NULL)
        return;
        
    free(matrix->N);
    free(matrix->J);
    free(matrix->I);
}

err_code allocate_memory_standart(matrix_t *matrix, const size_t rows, const size_t columns)
{
    double *data;

    matrix->rows = (size_t)rows;
    matrix->columns = (size_t)columns;

    matrix->ptrs = calloc(rows, sizeof(double *));
    if (matrix->ptrs == NULL)
    {
        return ERR_ALLOCATING_MEMORY;
    }

    data = calloc(rows * columns, sizeof(double));
    if (data == NULL)
    {
        free(matrix->ptrs);
        return ERR_ALLOCATING_MEMORY;
    }

    for (size_t i = 0; i < matrix->rows; i++)
        matrix->ptrs[i] = data + i * columns; 

    return ERR_SUCCESS;
}

err_code allocate_memory_csr(csr_matrix_t *matrix, const size_t rows, const size_t columns, const size_t nonzeros)
{
    if (nonzeros == 0)
    {
        matrix->N = NULL;
        matrix->J = NULL;
    }
    else
    {
        matrix->N = calloc(nonzeros, sizeof(double));
        if (matrix->N == NULL)
            return ERR_ALLOCATING_MEMORY;

        matrix->J = calloc(nonzeros, sizeof(int));
        if (matrix->J == NULL)
            return ERR_ALLOCATING_MEMORY;
    }

    matrix->I = calloc((rows + 1), sizeof(int));
    if (matrix->I == NULL)
        return ERR_ALLOCATING_MEMORY;

    matrix->rows = rows;
    matrix->columns = columns;

    return ERR_SUCCESS;
}

void print_standart(const matrix_t matrix)
{
    for (size_t i = 0; i < matrix.rows; i++)
    {
        for (size_t j = 0; j < matrix.columns; j++)
        {
            printf("%-10.2lf ", matrix.ptrs[i][j]);
        }
        printf("\n");
    }
    printf("Строк: %zu, столбцов: %zu\n", matrix.rows, matrix.columns);
    printf("\n");
}

void print_csr(const csr_matrix_t matrix)
{
    size_t nonzeros = matrix.I[matrix.rows];

    // Массив ненулевых элементов
    if (matrix.N == NULL)
    {
        puts("N = {}");
    }
    else
    {
        printf("N = {");
        for (size_t i = 0; i < nonzeros - 1; i++)
            printf("%.2lf, ", matrix.N[i]);
        printf("%.2lf}\n", matrix.N[nonzeros - 1]);
    }
    
    // Массив столбцов, соответствующих элементам из N
    if (matrix.J == NULL)
    {
        puts("J = {}");
    }
    else
    {
        printf("J = {");
        for (size_t i = 0; i < nonzeros - 1; i++)
            printf("%d, ", matrix.J[i]);
        printf("%d}\n", matrix.J[nonzeros - 1]);
    }

    // Массив содержит для каждой строки матрицы по индексу элемента из N, соответствующего началу этой строки
    printf("I = {");
    for (size_t i = 0; i < matrix.rows; i++)
        printf("%d, ", matrix.I[i]);
    printf("%d}\n", matrix.I[matrix.rows]);
    printf("Строк: %zu, столбцов: %zu\n", matrix.rows, matrix.columns);
    printf("\n");
}

err_code standart_to_csr(const matrix_t standart, csr_matrix_t *csr)
{
    size_t k = 0, total_nonzeros = 0;
    bool first_in_row = true;

    // Подсчет ненулевых элементов
    for (size_t i = 0; i < standart.rows; i++)
    {
        for (size_t j = 0; j < standart.columns; j++)
        {
            if (standart.ptrs[i][j] != 0)
                total_nonzeros++;
        }
    }

    // Выделение памяти в соответствии с количеством ненулевых элементов
    if (allocate_memory_csr(csr, standart.rows, standart.columns, total_nonzeros))
        return ERR_ALLOCATING_MEMORY;

    // Заполнение массивов N, J, I для матрицы в формате csr
    for (size_t i = 0; i < standart.rows; i++)
    {
        for (size_t j = 0; j < standart.columns; j++)
        {
            if (standart.ptrs[i][j] != 0)
            {
                csr->N[k] = standart.ptrs[i][j];
                csr->J[k] = j;

                if (first_in_row)
                {
                    csr->I[i] = k;
                    first_in_row = false;
                }
                k++;
            }
        }
        if (!first_in_row)
            first_in_row = true;
        else
            csr->I[i] = -1;
    }
    
    // В конце массива хранится количество ненулевых элементов
    csr->I[standart.rows] = k;

    // Заполняем поля нулевых строк
    for (int i = standart.rows - 2; i >= 0; i--)
    {
        if (csr->I[i] == -1)
            csr->I[i] = csr->I[i + 1];
    }

    return ERR_SUCCESS;
}

err_code csr_to_standart(const csr_matrix_t csr, matrix_t *standart)
{
    int col;
    
    if (allocate_memory_standart(standart, csr.rows, csr.columns))
        return ERR_ALLOCATING_MEMORY;

    if (csr.N == NULL || csr.J == NULL)
        return ERR_SUCCESS;

    for (int i = 0; i < (int)csr.rows; i++)
    {
        if (csr.I[i] < csr.I[i + 1])
        {
            for (int j = csr.I[i]; j < csr.I[i + 1]; j++)
            {
                col = csr.J[j];
                standart->ptrs[i][col] = csr.N[j];
            }
        }
    }
    return ERR_SUCCESS;
}

err_code standart_addition(const matrix_t matrix_1, const matrix_t matrix_2, matrix_t *result)
{    
    // Проверяем совпадение размерностей матриц
    if (matrix_1.rows != matrix_2.rows || matrix_1.columns != matrix_2.columns)
        return ERR_MTX_DIM_MISMATCH;
    
    // Поэлементное сложение
    for (size_t i = 0; i < matrix_1.rows; i++)
    {
        for (size_t j = 0; j < matrix_2.columns; j++)
        {
            result->ptrs[i][j] = matrix_1.ptrs[i][j] + matrix_2.ptrs[i][j];
        }
    }
    return ERR_SUCCESS;
}

err_code csr_addition(const csr_matrix_t A, const csr_matrix_t B, csr_matrix_t *C)
{
    int *IX = malloc(A.columns * sizeof(int)); // массив-переключатель для столбцов
    int j_pos = 0, col = 0;

    // Проверяем совпадение размерностей матриц
    if (A.rows != B.rows || A.columns != B.columns)
        return ERR_MTX_DIM_MISMATCH;

    if (A.J == NULL && A.N == NULL && B.J != NULL && B.N != NULL)
    {
        C->I = B.I;
        C->J = B.J;
        C->N = B.N;
        return ERR_SUCCESS;
    }

    if (B.J == NULL && B.N == NULL && A.J != NULL && A.N != NULL)
    {
        C->I = A.I;
        C->J = A.J;
        C->N = A.N;
        return ERR_SUCCESS;
    }

    if (B.J == NULL && B.N == NULL && A.J == NULL && A.N == NULL)
    {
        C->I = A.I;
        C->J = NULL;
        C->N = NULL;
        return ERR_SUCCESS;
    } 

    // Инициализация массива переключателя
    for (size_t i = 0; i < A.columns; i++)
        IX[i] = -1;

    // Главный цикл по строкам
    for (int i = 0; i < (int)A.rows; i++)
    {
        // Запись начального индекса для строки в матрице C
        C->I[i] = j_pos;

        // Добавляем элементы из матрицы A
        if (A.I[i] < A.I[i + 1])
        {
            for (int j = A.I[i]; j < A.I[i + 1]; j++)
            {
                col = A.J[j];
                C->N[j_pos] = A.N[j];
                C->J[j_pos] = col;
                IX[col] = j_pos; // Запоминаем индекс в C для этого столбца
                j_pos++;
            }
        }

        // Добавляем элементы из матрицы B и складываем значения, если столбец уже существует
        if (B.I[i] < B.I[i + 1])
        {
            for (int j = B.I[i]; j < B.I[i + 1]; j++)
            {
                col = B.J[j];
                
                if (IX[col] != -1) // Если столбец уже был добавлен из A
                {
                    C->N[IX[col]] += B.N[j];
                }
                else // Иначе добавляем новый элемент из B
                {
                    C->N[j_pos] = B.N[j];
                    C->J[j_pos] = col;
                    IX[col] = j_pos; // Запоминаем индекс в C для этого столбца
                    j_pos++;
                }
            }
        }

        // Сбрасываем переключатели для текущей строки
        for (int j = A.I[i]; j < A.I[i + 1]; j++)
            IX[A.J[j]] = -1;
        if (B.I[i] < B.I[i + 1])
            for (int j = B.I[i]; j < B.I[i + 1]; j++)
                IX[B.J[j]] = -1;
    }

    // Записываем количество ненулевых элементов в конец массива I
    C->I[A.rows] = j_pos;

    // Освобождаем память
    free(IX);

    return ERR_SUCCESS;
}

void shuffle(double *arr, const int elems_num)
{
    size_t j = 0;
    double temp = 0;

    srand((unsigned int)time(NULL));

    for (size_t i = elems_num - 1; i > 0; i--)
    {
        j = rand() % (i + 1);

        temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void init_standart(matrix_t *matrix, size_t nonzeros)
{
    size_t nums_in_row = 0, remainder = 0;

    srand((unsigned int)time(NULL));
    
    nums_in_row = nonzeros / matrix->rows;
    remainder = nonzeros % matrix->rows;
    
    for (size_t i = 0; i < matrix->rows; i++)
    {
        for (size_t j = 0; j < nums_in_row; j++)
        {
            matrix->ptrs[i][j] = ((double)rand() / RAND_MAX) * rand();
        }
        if (remainder--)
            matrix->ptrs[i][nums_in_row] = ((double)rand() / RAND_MAX) * rand();
        
        shuffle(matrix->ptrs[i], matrix->columns);
    }
}

unsigned long long count_standart_addition_time(const matrix_t matrix_1, const matrix_t matrix_2, matrix_t *result)
{
    struct timeval start, end;
    unsigned long long start_us, end_us;

    gettimeofday(&start, NULL);
    (void)standart_addition(matrix_1, matrix_2, result);
    gettimeofday(&end, NULL);

    start_us = (unsigned long long)start.tv_sec * 1000000ULL + start.tv_usec;
    end_us = (unsigned long long)end.tv_sec * 1000000ULL + end.tv_usec;

    return end_us - start_us;
}

unsigned long long count_csr_addition_time(const csr_matrix_t matrix_1, const csr_matrix_t matrix_2, csr_matrix_t *result)
{
    struct timeval start, end;
    unsigned long long start_us, end_us;

    gettimeofday(&start, NULL);
    (void)csr_addition(matrix_1, matrix_2, result);
    gettimeofday(&end, NULL);

    start_us = (unsigned long long)start.tv_sec * 1000000ULL + start.tv_usec;
    end_us = (unsigned long long)end.tv_sec * 1000000ULL + end.tv_usec;

    return end_us - start_us;
}

void set_matrix_to_zero(matrix_t *matrix)
{
    for (size_t i = 0; i < matrix->rows; i++)
    {
        for (size_t j = 0; j < matrix->columns; j++)
        {
            matrix->ptrs[i][j] = 0;
        }
    }
}

