#include "io.h"
#include <string.h>

void main_menu(void)
{
    puts("");
    horizontal_rule();

    puts("\nМЕНЮ\n");
    puts("1. Добавление студента.");
    puts("2. Удаление студента.");
    puts("3. Удаление студентов с низким баллом (<= 2.0).");
    puts("4. Поиск информации о студенте.");
    puts("5. Загрузка дерева из файла.");
    puts("6. Вывод дерева.");
    puts("7. Сравнение алгоритмов удаления.");
    puts("0. Выход.\n");

    horizontal_rule();
    puts("");
}

void horizontal_rule(void)
{
    puts("---------------------------------------------------------------------------------------------------------------------------------------------");
}

void input_arrow(void)
{
    printf(">>>>>> ");
}

err_code_e get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message) 
{
    char *buf = NULL;
    err_code_e rc;

    while (true)
    {
        printf("%s\n", prompt);
        input_arrow();

        if ((rc = read_string(stdin, &buf)) == ERR_EOF_REACHED) 
            break; 

        if (rc != ERR_SUCCESS) 
            continue;

        if (is_long_int(num, buf) && *num >= min_val && *num <= max_val) 
            break;

        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code_e get_string_from_stdin(char *prompt, char **string, char *error_message)
{
    err_code_e rc;

    *string = NULL;

    while (true)
    {
        printf("%s\n", prompt);
        input_arrow();

        if ((rc = read_string(stdin, string)) == ERR_EOF_REACHED) 
            break; 

        if (rc == ERR_SUCCESS) 
            break;

        printf(RED"%s\n\n"RESET, error_message);
    }

    return rc;
}

err_code_e get_double_from_stdin(char *prompt, double *num, double min_val, double max_val, char *error_message) 
{
    char *buf = NULL;
    err_code_e rc;

    while (true)
    {
        printf("%s\n", prompt);
        input_arrow();

        if ((rc = read_string(stdin, &buf)) == ERR_EOF_REACHED) 
            break; 

        if (rc != ERR_SUCCESS) 
            continue;

        if (is_double(num, buf) && *num >= min_val && *num <= max_val)
            break;
        
        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}
