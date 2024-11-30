#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "io.h"
#include "stack.h"
#include "colors.h"
#include "str.h"
#include "calculations.h"
#include "menu_actions.h"

typedef enum {LEAVE, ARR_STACK, LINKED_LIST_STACK, COMPARE} mode_e;

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
    node_t *stack_list = NULL;

    // Настройка функций для массивного и спискового стеков
    init_array_stack_funcs(&array_funcs, stack_arr);
    init_list_stack_funcs(&list_funcs, &stack_list);

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
                    process_actions(EXIT, &menu, NULL, current_funcs);
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
                    break;
            }
        } 
        else if (menu == STACK_OPERATIONS_MENU) 
        {
            stack_operations_menu();

            prompt = "Выберите действие (от 0 до 6): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 6. Повторите ввод.";
            if ((rc = get_int_from_stdin(prompt, &choice, 0, 6, error_message)) != ERR_SUCCESS)
                return process_error(rc);

            action = choice;
            if ((rc = process_actions(action, &menu, removed, current_funcs)) == ERR_EOF_REACHED)
                return rc;
        }
    }
    return ERR_SUCCESS;
}

