#include "menu_actions.h"

err_code operate_csr_addition(const csr_matrix_t csr_1, const csr_matrix_t csr_2, csr_matrix_t *csr_result)
{
    err_code rc = ERR_SUCCESS;
    matrix_t result;
    output_format_e format = CSR;

    if ((rc = allocate_memory_csr(csr_result, csr_1.rows, csr_1.columns, csr_1.I[csr_1.rows] + csr_2.I[csr_1.rows])))
        return rc;

    if ((rc = csr_addition(csr_1, csr_2, csr_result)) != ERR_SUCCESS)
        return rc;
    
    if ((rc = csr_to_standart(*csr_result, &result)) != ERR_SUCCESS)
        return rc;
    if ((rc = standart_to_csr(result, csr_result)) != ERR_SUCCESS)
        return rc;

    if (csr_result->rows <= 30 && csr_result->columns <= 30)
    {
        if ((rc = get_output_format(&format)) != ERR_SUCCESS)
            return rc;
    }

    puts("Результат сложения:");
    if (format == CSR)
        print_csr(*csr_result);
    else if (format == STANDART)
        print_standart(result);

    free(result.ptrs);
    free_csr(csr_result);

    return rc;
}

err_code operate_standart_addition(const matrix_t standart_1, const matrix_t standart_2, matrix_t *result)
{
    err_code rc = ERR_SUCCESS;
    csr_matrix_t csr_result;
    output_format_e format = STANDART;

    if ((rc = allocate_memory_standart(result, standart_1.rows, standart_1.columns)) != ERR_SUCCESS)
        return rc;

    if ((rc = standart_addition(standart_1, standart_2, result)) != ERR_SUCCESS)
        return rc;

    if ((rc = standart_to_csr(*result, &csr_result)) != ERR_SUCCESS)
        return rc;

    if (result->rows <= 30 && result->columns <= 30)
    {
        if ((rc = get_output_format(&format)) != ERR_SUCCESS)
            return rc;
    }

    puts("Результат сложения:");
    if (format == CSR)
        print_csr(csr_result);
    else if (format == STANDART)
        print_standart(*result);

    free(result->ptrs);
    free_csr(&csr_result);

    return rc;
}

err_code operate_algorithms_compare(void)
{
    size_t rows = 0, columns = 0, nonzeros = 0, total_nonzeros = 0;
    size_t iterations = 100;
    matrix_t standart_1, standart_2, result;
    csr_matrix_t csr_1, csr_2, csr_result;
    err_code rc = ERR_SUCCESS;
    long long standart_size = 0, csr_size = 0;
    long long addition_standart_time = 0, addition_csr_time = 0;
    double time_difference = 0, size_difference = 0;

    for (size_t n = 250; n <= 1000; n += 250)
    {
        printf(BOLD "Матрица %zux%zu\n", n, n);
        rows = n;
        columns = n; 

        puts("------------------------------------------------------------------------------------------------------------------");
        printf("|%-10s|%-70s|%-60s|\n", "", "Скорость обработки, мкс", "Размер, байт");
        printf("|    %s     |------------------------------------------------------------------------------------------------------\n", "%");
        printf("|%-10s|%-23s|%-16s|%-26s|%-23s|%-16s|%-26s|\n", "", "Обычная", "CSR", "Разница, раз", "Обычная", "CSR", "Разница, раз");
        puts("------------------------------------------------------------------------------------------------------------------" RESET);


        for (size_t i = 1; i <= 100; i++)
        {
            nonzeros = i * rows * columns / 100;

            // Для обычных матриц
            if ((rc = allocate_memory_standart(&standart_1, rows, columns)) != ERR_SUCCESS ||
                (rc = allocate_memory_standart(&standart_2, rows, columns)) != ERR_SUCCESS ||
                (rc = allocate_memory_standart(&result, rows, columns)) != ERR_SUCCESS)
            {
                free_standart(&standart_1);
                free_standart(&standart_2);
                free_standart(&result);
                return rc;
            }

            addition_standart_time = 0;
            addition_csr_time = 0;

            for (size_t j = 0; j < iterations; j++)
            {
                set_matrix_to_zero(&standart_1);
                set_matrix_to_zero(&standart_2);
                
                init_standart(&standart_1, nonzeros);
                init_standart(&standart_2, nonzeros);

                addition_standart_time += count_standart_addition_time(standart_1, standart_2, &result);
                
                // Для формата CSR
                if ((rc = standart_to_csr(standart_1, &csr_1)) != ERR_SUCCESS ||
                    (rc = standart_to_csr(standart_2, &csr_2)) != ERR_SUCCESS)
                {
                    free_standart(&standart_1);
                    free_standart(&standart_2);
                    free_standart(&result);
                    free_csr(&csr_1);
                    free_csr(&csr_2);
                    return rc;
                }

                total_nonzeros = csr_1.I[csr_1.rows] + csr_2.I[csr_2.rows];
                if ((rc = allocate_memory_csr(&csr_result, rows, columns, total_nonzeros)) != ERR_SUCCESS)
                {
                    free(standart_1.ptrs);
                    free(standart_2.ptrs);
                    free(result.ptrs);
                    free_csr(&csr_1);
                    free_csr(&csr_2);
                    return rc;
                }
                
                addition_csr_time += count_csr_addition_time(csr_1, csr_2, &csr_result);

                free_csr(&csr_1);
                free_csr(&csr_2);
                free_csr(&csr_result);
            }

            standart_size = sizeof(matrix_t) + rows * columns * sizeof(double) + rows * sizeof(double *);
            csr_size = sizeof(csr_matrix_t) + (rows + 1) * sizeof(int) + nonzeros * sizeof(double) + nonzeros * sizeof(int);

            time_difference = (double)addition_standart_time / addition_csr_time;
            size_difference = (double)standart_size / csr_size;

            free_standart(&standart_1);
            free_standart(&standart_2);
            free_standart(&result);

            printf("|%-10zu|%-16lld|%-16lld|%-16.2f|%-16lld|%-16lld|%-16.2f|\n", i, addition_standart_time / iterations, addition_csr_time / iterations, time_difference, standart_size, csr_size, size_difference);
            puts("------------------------------------------------------------------------------------------------------------------");
        }
        puts("");
    }

    return ERR_SUCCESS;
}

err_code operate_matrix_reading(matrix_t *matrix, csr_matrix_t *csr_matrix)
{   
    err_code rc = ERR_SUCCESS;

    do
    {
        if ((rc = get_matrix(matrix)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }
        rc = standart_to_csr(*matrix, csr_matrix);
    } while (rc != ERR_SUCCESS && rc != ERR_EOF_REACHED);  

    return rc;       
}

err_code operate_matrix_output(const matrix_t matrix, const csr_matrix_t csr_matrix)
{
    err_code rc = ERR_SUCCESS;
    output_format_e format = STANDART;

    if (matrix.rows <= 30 && matrix.columns <= 30)
    {
        if ((rc = get_output_format(&format)) != ERR_SUCCESS)
            return rc;
    }

    if (format == STANDART)
        print_standart(matrix);
    else if (format == CSR)
        print_csr(csr_matrix);
    
    return rc;
}

