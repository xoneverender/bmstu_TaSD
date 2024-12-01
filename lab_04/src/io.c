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
    puts("3. Вычисление значения выражения, содержащегося в стеке.");
    puts("4. Вычисление значения выражения в введенной строке.");
    puts("5. Просмотр стека.");
    puts("6. Просмотр удаленных элементов.");
    puts("7. Возврат в главное меню.");
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

        if (is_long_int(num, buf) && *num >= min_val && *num <= max_val) 
            break;

        printf(RED"%s\n\n"RESET, error_message);
    }

    free(buf);
    return rc;
}

err_code_e process_actions(const action_e action, menu_e *menu, removed_t *removed, action_funcs_t *funcs, mode_e mode)
{
    err_code_e rc = ERR_SUCCESS;

    puts("");
    horizontal_rule();

    switch (action)
    {
        case ADD_ELEM:
            puts("\nДОБАВЛЕНИЕ ЭЛЕМЕНТА В СТЕК\n");

            puts(BOLD "Стек до добавления элемента: \n" RESET);
            funcs->print(funcs->stack);

            if ((rc = operate_adding_element(funcs->stack, funcs->push)))
                process_error(rc);
            else
            {
                puts(BOLD "Стек после добавления элемента: \n" RESET);
                funcs->print(funcs->stack);
                puts(GREEN"Добавление элемента выполнено успешно."RESET);
            }
            break;
        case REMOVE_ELEM:
            puts("\nУДАЛЕНИЕ ЭЛЕМЕНТА ИЗ СТЕКА\n");

            puts(BOLD "Стек до удаления элемента: \n" RESET);
            funcs->print(funcs->stack);

            if ((rc = operate_removing_element(funcs->stack, removed, funcs->pop)))
                process_error(rc);
            else
            {
                puts(BOLD "Стек после удаления элемента: \n" RESET);
                funcs->print(funcs->stack);

                puts(GREEN"Элемент успешно удален."RESET);
            }
            break;
        case CALCULATE_STACK:
            puts("\nВЫЧИСЛЕНИЕ ЗНАЧЕНИЯ ВЫРАЖЕНИЯ\n");

            if ((rc = operate_calculating_rpn(funcs->stack, funcs->pop, funcs->push, funcs->free, mode)))
                process_error(rc);
            else
                puts(GREEN"Значение выражения вычислено успешно."RESET);
            break;
        case CALCULATE_STR:
            puts("\nВЫЧИСЛЕНИЕ ЗНАЧЕНИЯ ВЫРАЖЕНИЯ В ВЕДЕННОЙ СТРОКЕ\n");

            if ((rc = operate_calculating_rpn_from_string(funcs->stack, funcs->pop, funcs->push, funcs->free, mode)))
                process_error(rc);
            else
                puts(GREEN"Значение выражения вычислено успешно."RESET);
            break;
        case SHOW_STACK:
            puts("\nПРОСМОТР СТЕКА\n");
            funcs->print(funcs->stack);
            break;
        case SHOW_REMOWED:
            puts("\nПРОСМОТР АДРЕСОВ УДАЛЕННЫХ ЭЛЕМЕНТОВ\n");
            operate_printing_removed(removed);
            break;
        case BACK_TO_MAIN_MENU:
            puts("\nВозвращение в главное меню...");
            *menu = MAIN_MENU;
            break;
        case EXIT:
            puts("\nВыход из программы...\n");
            if (funcs)
            {
                if (funcs->free)
                    funcs->free(funcs->stack);
                // free(funcs);
            }
            exit(0);
            break;
    }
    return rc;
}
