#include "io.h"

void horizontal_rule(void)
{
    puts("-----------------------------------------------------------------------------------------------------------------------");
}

void input_arrow(void)
{
    printf(">>>>>> ");
}

void main_menu(void)
{
    puts("");
    horizontal_rule();

    puts("\nМЕНЮ\n");
    puts("1. Сложение матриц в обычном формате.");
    puts("2. Сложение матриц в формате CSR.");
    puts("3. Сравнение алгоритмов обработки матриц.");
    puts("4. Просмотр матриц.");
    puts("5. Замена первой матрицы.");
    puts("6. Замена второй матрицы.");
    puts("0. Выход.\n");

    horizontal_rule();
    puts("");
}

void first_iteration_menu(void)
{
    puts("");
    horizontal_rule();

    puts("\nМЕНЮ\n");
    puts("1. Сравнение алгоритмов обработки матриц.");
    puts("2. Загрузка матриц.");
    puts("0. Выход.\n");

    horizontal_rule();
    puts("");    
}

err_code get_output_format(output_format_e *format)
{
    char *prompt, *error_message;
    err_code rc = ERR_SUCCESS;
    long num = *format;

    puts("Выберите формат, в котором матрица будет выведена: ");
    puts("1. Обычный формат");
    puts("2. CSR\n");

    prompt = "Выберите действие (от 1 до 2): ";
    error_message = "Ошибка: введите целое число от 1 до 2.";
    rc = get_int_from_stdin(prompt, &num, 1, 2, error_message);
    puts("");

    *format = num;
    return rc;
} 

err_code get_input_format(input_format_e *format)
{
    char *prompt, *error_message;
    err_code rc = ERR_SUCCESS;
    long num = *format;

    puts("Выберите формат, в котором матрица будет считана: ");
    puts("1. Обычный");
    puts("2. Координатный\n");

    prompt = "Выберите действие (от 1 до 2): ";
    error_message = "Ошибка: введите целое число от 1 до 2.";
    rc = get_int_from_stdin(prompt, &num, 1, 2, error_message);
    puts("");

    *format = num;
    return rc;
} 

err_code get_matrix_source(source_e *source)
{
    char *prompt, *error_message;
    err_code rc = ERR_SUCCESS;
    long num;

    puts("Выберите способ получения матрицы:");
    puts("1. Из файла");
    puts("2. Ввод с клавиатуры\n");

    // Получение пользовательского выбора
    prompt = "Выберите действие (от 1 до 2): ";
    error_message = "Ошибка: введите целое число от 1 до 2.";
    rc = get_int_from_stdin(prompt, &num, 1, 2, error_message);

    *source = num;
    return rc;
}

err_code get_matrix(matrix_t *matrix)
{   
    err_code rc = ERR_SUCCESS;
    source_e source;
    char *filename = NULL;

    if ((rc = get_matrix_source(&source)) != ERR_SUCCESS)
        return rc;

    if (source == TXTFILE)
    {
        do
        {
            if ((rc = get_filename(&filename)) != ERR_SUCCESS)
            return rc;
        
            rc = read_matrix_from_file(filename, matrix);

            process_error(rc);
        } while (rc != ERR_SUCCESS && rc != ERR_EOF_REACHED);
    
        free(filename);
    }
    else if (source == STDIN)
    {
        rc = read_matrix_from_stdin(matrix);
    }

    return rc;
}

err_code read_matrix_from_stdin(matrix_t *matrix)
{
    input_format_e format = STANDARD;
    err_code rc = ERR_SUCCESS;
    long rows = 0, columns = 0;
    char *prompt, *error_message;

    do
    {
        // Получение количества строк
        prompt = "\nВведите количество строк в матрице:";
        error_message = "Ошибка: должно быть введено целое положительное число.";
        if ((rc = get_int_from_stdin(prompt, &rows, 1, INT_MAX, error_message)) != ERR_SUCCESS) 
            return rc;

        // Получение количества столбцов
        prompt = "Введите количество столбцов в матрице:";
        error_message = "Ошибка: должно быть введено целое положительное число.";
        if ((rc = get_int_from_stdin(prompt, &columns, 1, INT_MAX, error_message)) != ERR_SUCCESS) 
            return rc;
        
        puts("");

        if ((rc = allocate_memory_standart(matrix, (size_t)rows, (size_t)columns)) != ERR_SUCCESS) 
            process_error(rc);
    } while (rc);


    // Определение формата ввода на основе размерности матрицы
    if (rows >= 30 && columns >= 30)
    {
        format = COORDINATE;
    }
    else
    {
        if ((rc = get_input_format(&format)) != ERR_SUCCESS)
            return rc;
    }
    
    if (format == COORDINATE)
    {
        if ((rc = read_matrix_coordinate(matrix, rows, columns)))
            return rc;
    }
    else if (format == STANDARD)
    {
        if ((rc = read_matrix_standart(matrix, rows, columns)))
            return rc;
    }

    return rc;
}

err_code read_matrix_coordinate(matrix_t *matrix, const long rows, const long columns)
{
    err_code rc = ERR_SUCCESS;
    bool nonzero = true;
    char *prompt, *error_message;
    long total_nonzero = 0, current_row = 0, current_column = 0;
    double fnum = 0;

    prompt = "Введите количество ненулевых элементов матрицы:";
    error_message = "Ошибка: количество ненулевых элементов должно быть целым числом в пределах размера матрицы.";
    if ((rc = get_int_from_stdin(prompt, &total_nonzero, 0, rows * columns, error_message)) != ERR_SUCCESS) 
    {
        free_standart(matrix);
        return rc;
    }

    for (size_t i = 0; i < (size_t)total_nonzero; i++)
    {
        puts("");
        prompt = "Введите номер строки:";
        error_message = "Ошибка: должно быть введено целое число. Число должно быть меньше количества строк в матрице.";
        if ((rc = get_int_from_stdin(prompt, &current_row, 0, matrix->rows - 1, error_message)) != ERR_SUCCESS) 
        {
            free_standart(matrix);
            return rc;
        }

        prompt = "Введите номер столбца:";
        error_message = "Ошибка: должно быть введено целое число. Число должно быть меньше количества столбцов в матрице.";
        if ((rc = get_int_from_stdin(prompt, &current_column, 0, matrix->columns - 1, error_message)) != ERR_SUCCESS) 
        {
            free_standart(matrix);
            return rc;
        }

        if (matrix->ptrs[current_row][current_column] != 0)
        {
            puts(RED "Элемент матрицы по этому индексу уже содержит ненулевое значение." RESET);
            i--;
            continue;
        }

        prompt = "Введите элемент матрицы:";
        error_message = "Ошибка: должно быть введено ненулевое вещественное число.";
        if ((rc = get_double_from_stdin(prompt, &fnum, error_message, nonzero = true)) != ERR_SUCCESS) 
        {
            free_standart(matrix);
            return rc;
        }            

        matrix->ptrs[current_row][current_column] = fnum;
    } 

    return rc;
}

err_code read_matrix_standart(matrix_t *matrix, const long rows, const long columns)
{
    err_code rc = ERR_SUCCESS;
    bool nonzero = true;
    char *prompt, *error_message;
    double fnum = 0;

    for (size_t i = 0; i <(size_t)rows; i++)
    {
        for (size_t j = 0; j < (size_t)columns; j++)
        {
            printf("%zu строка %zu столбец. ", i + 1, j + 1);
            prompt = "Введите элемент матрицы:";
            error_message = "Ошибка: должно быть введено вещественное число.";
            if ((rc = get_double_from_stdin(prompt, &fnum, error_message, nonzero = false)) != ERR_SUCCESS) 
            {
                free_standart(matrix);
                return rc;
            }                
            matrix->ptrs[i][j] = fnum;
            puts("");
        }
    }

    return rc;
}


err_code get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message) 
{
    char *buf = NULL;
    size_t len = STRING_LEN;
    err_code rc;

    while (true)
    {
        printf("%s\n", prompt);
        input_arrow();

        if ((rc = read_string(stdin, &buf, &len)) == ERR_EOF_REACHED) 
            break; 

        if (rc != ERR_SUCCESS) 
            continue;

        if (is_whole_number(num, buf) && *num >= min_val && *num <= max_val) 
            break;

        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code get_double_from_stdin(char *prompt, double *num, char *error_message, bool nonzero) 
{
    char *buf = NULL;
    size_t len = STRING_LEN;
    err_code rc;

    while (true)
    {
        printf("%s\n", prompt);
        input_arrow();

        if ((rc = read_string(stdin, &buf, &len)) == ERR_EOF_REACHED) 
            break; 

        if (rc != ERR_SUCCESS) 
            continue;

        if (nonzero)
        {
            if (is_double(num, buf) && fabs(*num) > 1e-9) 
                break;
        }
        else
        {
            if (is_double(num, buf))
                break;
        }

        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code get_filename(char **filename) 
{
    err_code rc;
    size_t len = STRING_LEN;

    do 
    {
        puts("\nВведите название файла, содержащего матрицу: ");
        input_arrow();

        rc = read_string(stdin, filename, &len);
        
        if (rc == ERR_EOF_REACHED)
            break;

        if (rc == ERR_INVALID_INPUT) 
        {
            process_error(rc);
            continue;
        }

        rc = open_file(*filename);

        if (rc)
            process_error(rc);
    } while (rc);
    return rc;
}

err_code process_actions(const action_e action, matrix_t *standart_1, matrix_t *standart_2, csr_matrix_t *csr_1, csr_matrix_t *csr_2)
{
    matrix_t result;
    csr_matrix_t csr_result;
    err_code rc;

    puts("");
    horizontal_rule();

    switch (action) 
    {
        case EXIT:
            free_standart(standart_1);
            free_standart(standart_2);
            free_csr(csr_1);
            free_csr(csr_2);
            puts("Выход...");
            exit(0);
        case SUM_STANDART:
            puts("\nСЛОЖЕНИЕ МАТРИЦ В ОБЫЧНОМ ФОРМАТЕ\n");
            if ((rc = operate_standart_addition(*standart_1, *standart_2, &result)))
                return rc;
            puts(GREEN "\nСложение выполнено успешно.\n" RESET);
            break;
        case SUM_CSR:
            puts("\nСЛОЖЕНИЕ МАТРИЦ В ФОРМАТЕ CSR\n");
            if ((rc = operate_csr_addition(*csr_1, *csr_2, &csr_result)) != ERR_SUCCESS)
                return rc;
            puts(GREEN "Сложение выполнено успешно.\n" RESET);
            break;
        case COMPARE_ALGS:
            puts("\nСРАВНЕНИЕ АЛГОРИТМОВ СЛОЖЕНИЯ ДЛЯ ДАННЫХ В РАЗЛИЧНЫХ ФОРМАТАХ\n");
            if ((rc = operate_algorithms_compare()))
                return rc;
            puts(GREEN "\nСравнение алгоритмов выполнено успешно.\n" RESET);
            break;
        case VIEW_MATRICES:
            puts("\nПРОСМОТР ИСХОДНЫХ МАТРИЦ\n");
            puts(BOLD "Матрица №1\n" RESET);
            if ((rc = operate_matrix_output(*standart_1, *csr_1)))
                return rc;
            puts(BOLD "Матрица №2\n" RESET);
            if ((rc = operate_matrix_output(*standart_2, *csr_2)))
                return rc;
            puts(GREEN "\nВывод матриц выполнен успешно.\n" RESET);
            break;
        case READ_FIRST_MATRIX:
            puts("\nЗАПИСЬ МАТРИЦЫ №1\n");
            if ((rc = operate_matrix_reading(standart_1, csr_1)))
                return rc;
            puts(GREEN "Матрица успешно считана." RESET);
            break;
        case READ_SECOND_MATRIX:
            puts("\nЗАПИСЬ МАТРИЦЫ №2\n");
            if ((rc = operate_matrix_reading(standart_2, csr_2)))
                return rc;
            puts(GREEN "Матрица успешно считана." RESET);
            break;
    }
    return rc;
}