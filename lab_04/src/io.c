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
    puts("1. Работа со стеком в виде статического массива.");
    puts("2. Работа со стеком в виде односвязного списка.");
    puts("3. Сравнение реализаций.");
    puts("0. Выход.\n");

    horizontal_rule();
    puts("");
}

void stack_operations_menu(void)
{
    puts("");
    horizontal_rule();

    puts("\nМЕНЮ\n");
    puts("1. Добавление элемента в стек.");
    puts("2. Удаление элемента из стека.");
    puts("3. Вычисление значения выражения.");
    puts("4. Просмотр стека.");
    puts("5. Просмотр удаленных элементов.");
    puts("6. Возврат главное меню.");
    puts("0. Выход.\n");

    horizontal_rule();
    puts("");
}

err_code_e get_int_from_stdin(char *prompt, long *num, int min_val, int max_val, char *error_message) 
{
    char *buf = NULL;
    size_t len = 0;
    err_code_e rc;

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

