#include "io.h"

void main_menu(void)
{
    puts("");
    horizontal_rule();

    puts("\nМЕНЮ\n");
    puts("1. Сравнение двух реализаций очереди.");
    puts("2. Запуск моделирования.");
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

        if (is_long_int(num, buf) && *num >= min_val && *num <= max_val) 
            break;

        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code_e get_double_from_stdin(char *prompt, double *num, double min_val, char *error_message) 
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

        if (is_double(num, buf) && *num >= min_val)
            break;
        
        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code_e process_action(action_e action)
{
    err_code_e rc = ERR_SUCCESS;

    puts("");
    horizontal_rule();

    switch (action)
    {
        case EXIT:
            puts("\nВыход из программы...\n");
            exit(0);
            break;
        case COMPARE:
            printf("\nСРАВНЕНИЕ ЭФФЕКТИВНОСТИ ОЧЕРЕДИ НА СТАТИЧЕСКОМ МАССИВЕ И ОЧЕРЕДИ НА СВЯЗНОМ СПИСКЕ\n");

            rc = compare();

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nСравнение реализаций выполнено успешно." RESET);
            break;
        case SIMULATE:
            printf("\nМОДЕЛИРОВАНИЯ ПРОЦЕССА ОБСЛУЖИВАНИЯ ПЕРВЫХ %d ЗАЯВОК ПЕРВОГО ТИПА\n\n", MAX_REQUESTS);

            rc = simulate();

            if (rc)
                process_error(rc);
            else
                puts(GREEN "\nСимуляция выполнена успешно." RESET);
            break;
    }

    return rc;
}