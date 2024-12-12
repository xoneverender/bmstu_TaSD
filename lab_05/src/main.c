#include "queue.h"
#include "errors.h"
#include "str.h"
#include "simulation.h"
#include "io.h"
#include "simulation.h"

int main(void)
{
    err_code_e rc = ERR_SUCCESS;
    action_e action = EXIT;
    char *prompt, *error_message;
    long choice;

    while (true)
    {
        main_menu();
        
        prompt = "Выберите действие (от 0 до 2): ";
        error_message = "Выбранный пункт меню должен быть целым числом от 0 до 2. Повторите ввод.";
        if ((rc = get_int_from_stdin(prompt, &choice, 0, 2, error_message)) != ERR_SUCCESS)
            return process_error(rc);
        action = choice;

        if ((rc = process_action(action)) != ERR_SUCCESS)
            return rc;
    }
}
