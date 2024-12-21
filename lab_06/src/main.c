#include <stdbool.h>
#include "errors.h"
#include "io.h"
#include "actions_processing.h"

int main(void)
{
    tree_node_t *root = NULL;
    err_code_e rc = ERR_SUCCESS;
    action_e action = EXIT;
    char *prompt, *error_message;
    long choice;

    while (true)
    {
        main_menu();
        
        prompt = "Выберите действие (от 0 до 7): ";
        error_message = "Выбранный пункт меню должен быть целым числом от 0 до 7. Повторите ввод.";
        if ((rc = get_int_from_stdin(prompt, &choice, 0, 7, error_message)) != ERR_SUCCESS)
            return process_error(rc);
        action = choice;

        if ((rc = process_action(action, &root)) == ERR_EOF_REACHED)
            return rc;
    }
}
