
#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "io.h"

typedef enum {LEAVE, COMPARE, GET_MATRICES} first_action_e;

int main(void)
{
    matrix_t standart_1, standart_2;
    csr_matrix_t csr_1, csr_2;
    first_action_e first_action = LEAVE;
    action_e action = EXIT;
    err_code rc = ERR_SUCCESS;
    bool matrices_given = false;
    char *prompt, *error_message;
    long num;

    while (true)
    {
        if (!matrices_given)
        {
            first_iteration_menu();

            prompt = "Выберите действие (от 0 до 2): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 2. Повторите ввод.";
            if ((rc = get_int_from_stdin(prompt, (long *)&first_action, 0, 2, error_message)) != ERR_SUCCESS)
                return rc;

            switch (first_action)
            {
                case LEAVE:
                    return rc;
                case COMPARE:
                    rc = process_actions(COMPARE_ALGS, &standart_1, &standart_2, &csr_1, &csr_2);
                    break;
                case GET_MATRICES:
                    if ((rc = process_actions(READ_FIRST_MATRIX, &standart_1, &standart_2, &csr_1, &csr_2)) != ERR_SUCCESS)
                        break;
                    if ((rc = process_actions(READ_SECOND_MATRIX, &standart_1, &standart_2, &csr_1, &csr_2)) != ERR_SUCCESS)
                        break;
                    matrices_given = true;
                    break;
            }
        }
        else
        {
            main_menu();

            // Получение пользовательского выбора
            prompt = "Выберите действие (от 0 до 6): ";
            error_message = "Выбранный пункт меню должен быть целым числом от 0 до 6. Повторите ввод.";
            rc = get_int_from_stdin(prompt, &num, 0, 6, error_message);
            if (rc == ERR_EOF_REACHED)
                process_actions(EXIT, &standart_1, &standart_2, &csr_1, &csr_2);
            action = num;
            rc = process_actions(action, &standart_1, &standart_2, &csr_1, &csr_2);
        }

        process_error(rc);

        if (rc == ERR_EOF_REACHED)
            process_actions(EXIT, &standart_1, &standart_2, &csr_1, &csr_2);
    }
}