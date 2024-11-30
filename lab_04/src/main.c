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
    action_funcs_t *funcs = NULL;
    removed_t *removed = NULL;
    char *prompt, *error_message;
    long choice;
    array_stack_t *stack_arr = malloc(sizeof(array_stack_t));
    stack_arr->top = 0;
    node_t *stack_list = NULL;

    funcs = malloc(sizeof(action_funcs_t));
    funcs->free = NULL;
    funcs->pop = NULL;
    funcs->print = NULL;
    funcs->push = NULL;
    funcs->stack = NULL;

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
                    process_actions(EXIT, &menu, NULL, funcs);
                    break;
                case ARR_STACK:
                    menu = STACK_OPERATIONS_MENU;
                    funcs->push = push_into_arr;
                    funcs->pop = pop_from_array;
                    funcs->free = free_array;
                    funcs->print = operate_printing_array_stack;
                    funcs->stack = stack_arr;
                    break;
                case LINKED_LIST_STACK:
                    menu = STACK_OPERATIONS_MENU;
                    funcs->push = push_into_list;
                    funcs->pop = pop_from_list;
                    funcs->free = free_list;
                    funcs->print = operate_printing_list_stack;
                    funcs->stack = &stack_list;
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
            if ((rc = get_int_from_stdin(prompt, (long *)&action, 0, 6, error_message)) != ERR_SUCCESS)
                return process_error(rc);

            if ((rc = process_actions(action, &menu, removed, funcs)) == ERR_EOF_REACHED)
                return rc;
        }
    }
}