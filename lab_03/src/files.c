#include "files.h"

err_code open_file(const char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return ERR_OPENING_FILE;

    fclose(f);

    return ERR_SUCCESS;
}

err_code read_matrix_dimensions(FILE *f, int *rows, int *columns) 
{
    char *buf = NULL, *token;
    size_t len = STRING_LEN;
    long num = 0;

    if (read_string(f, &buf, &len) != ERR_SUCCESS)
        return ERR_INVALID_FILE_FORMAT;

    token = strtok(buf, " ");
    if (!token || !is_whole_number(&num, token)) 
    {
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    if ((*rows = atol(token)) <= 0)
    {
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    token = strtok(NULL, " ");
    if (!token || !is_whole_number(&num, token))
    { 
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    if ((*columns = atol(token)) <= 0)
    { 
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    token = strtok(NULL, " ");
    if (token)
    {
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    free(buf);
    return ERR_SUCCESS;
}

err_code read_matrix_data(FILE *f, matrix_t *matrix) 
{
    char *buf = NULL, *token;
    double num = 0;
    err_code rc;
    size_t len = 0;

    for (size_t i = 0; i < matrix->rows; i++) 
    {
        // Чтение строки с данными для текущей строки матрицы
        if ((rc = read_string(f, &buf, &len)) != ERR_SUCCESS) 
        {
            free(matrix->ptrs);
            free(buf);
            return process_error(rc);
        }

        // Разбиение строки на токены и заполнение матрицы
        token = strtok(buf, " ");
        for (size_t j = 0; j < matrix->columns; j++) 
        {
            if (!token || !is_double(&num, token)) 
            {
                free(matrix->ptrs);
                free(buf);
                return ERR_INVALID_FILE_FORMAT;
            }

            matrix->ptrs[i][j] = num;
            token = strtok(NULL, " ");
        }

        // Проверка, что больше нет токенов в строке
        if (token) 
        {
            free(matrix->ptrs);
            free(buf);
            return ERR_INVALID_FILE_FORMAT;
        }
    }

    // Проверка, что в файле больше ничего нет
    if ((rc = read_string(f, &buf, &len)) == ERR_SUCCESS) 
    {
        free(matrix->ptrs);
        free(buf);
        return ERR_INVALID_FILE_FORMAT;
    }

    free(buf);
    return ERR_SUCCESS;
}


err_code read_matrix_from_file(const char *filename, matrix_t *matrix)
{
    err_code rc = ERR_SUCCESS;
    int rows, columns;

    FILE *f = fopen(filename, "r");
    if (!f)
        return ERR_OPENING_FILE;

    // Получаем размерность матрицы
    if ((rc = read_matrix_dimensions(f, &rows, &columns)) != ERR_SUCCESS) 
    {
        fclose(f);
        return rc;
    }

    // Инициализация матрицы с полученной размерностью
    if ((rc = allocate_memory_standart(matrix, (size_t)rows, (size_t)columns)) != ERR_SUCCESS) 
    {
        fclose(f);
        return rc;
    }

    // Чтение данных матрицы
    if ((rc = read_matrix_data(f, matrix)) != ERR_SUCCESS) 
    {
        fclose(f);
        return rc;
    }

    fclose(f);
    return ERR_SUCCESS;
}
