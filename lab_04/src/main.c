#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "io.h"
#include "stack.h"
#include "colors.h"

typedef enum {MAIN_MENU, INIT_MENU} menu_e;
typedef enum {LEAVE, ARR_STACK, LINKED_LIST_STACK, COMPARE} mode_e;

int main(void)
{
    err_code_e rc = ERR_SUCCESS;
    menu_e menu = INIT_MENU;
    mode_e mode;
    char *prompt, *error_message;

    while (true)
    {
        if (menu == INIT_MENU)
        {
            main_menu();

            prompt = "Выберите действие (от 0 до 3): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 3. Повторите ввод.";
            if ((rc = get_int_from_stdin(prompt, (long *)&mode, 0, 3, error_message)) != ERR_SUCCESS)
                return rc;
        }
    }
}