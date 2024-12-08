#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "io.h"
#include "stack.h"
#include "colors.h"
#include "str.h"
#include "calculations.h"
#include "menu_actions.h"

int main(void) 
{
    err_code_e rc = ERR_SUCCESS;
    menu_e menu = MAIN_MENU;
    mode_e mode = LEAVE;
    action_e action = EXIT;
    action_funcs_t array_funcs, list_funcs;
    removed_t *removed = NULL;
    char *prompt, *error_message;
    long choice;

    // Инициализация массивного стека
    array_stack_t *stack_arr = malloc(sizeof(array_stack_t));
    if (!stack_arr) 
        return process_error(rc);
    stack_arr->top = 0;

    // Инициализация спискового стека
    list_stack_t *stack_list = NULL;

    // Настройка функций для массивного и спискового стеков
    init_array_stack_funcs(&array_funcs, stack_arr);
    init_list_stack_funcs(&list_funcs, &stack_list);

    removed = malloc(sizeof(removed_t));
    removed->capacity = 0;
    removed->count = 0;
    removed->data = NULL;

    // Указатель на текущую структуру функций
    action_funcs_t *current_funcs = NULL; 

    while (true) 
    {
        if (menu == MAIN_MENU) 
        {
            main_menu();

            prompt = "Выберите действие (от 0 до 3): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 3. Повторите ввод.";
            if ((rc = get_int_from_stdin(prompt, &choice, 0, 3, error_message)) != ERR_SUCCESS)
                return process_error(rc);
            mode = choice;

            switch (mode) 
            {
                case LEAVE:
                    process_actions(EXIT, &menu, NULL, current_funcs, mode);
                    break;
                case ARR_STACK:
                    menu = STACK_OPERATIONS_MENU;
                    current_funcs = &array_funcs;
                    break;
                case LINKED_LIST_STACK:
                    menu = STACK_OPERATIONS_MENU;
                    current_funcs = &list_funcs;
                    break;
                case COMPARE:
                    puts("");
                    horizontal_rule();
                    puts("\nСРАВНЕНИЕ ПРОИЗВОДИТЕЛЬНОСТИ СТЕКА НА МАССИВЕ И СТЕКА НА СПИСКЕ\n");
                    rc = operate_comparizon();
                    process_error(rc);
                    break;
            }
        } 
        else if (menu == STACK_OPERATIONS_MENU) 
        {
            stack_operations_menu();

            prompt = "Выберите действие (от 0 до 7): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 7. Повторите ввод.";
            if ((rc = get_int_from_stdin(prompt, &choice, 0, 7, error_message)) != ERR_SUCCESS)
                return process_error(rc);

            action = choice;
            if ((rc = process_actions(action, &menu, removed, current_funcs, mode)) == ERR_EOF_REACHED)
                return rc;
        }
    }
    return ERR_SUCCESS;
}

