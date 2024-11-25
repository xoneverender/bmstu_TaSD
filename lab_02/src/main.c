#include <stdio.h>
#include <string.h>
#include "structs.h"
#include "consts.h"
#include "errors.h"
#include "menu_actions.h"
#include "user_interface.h"
#include "files.h"
#include "str.h"

int main(void)
{
    char filename[MAX_STRING_LEN + 2];
    err_code rc = 0;
    long choice = 0;
    book_t library_init[1000];
    key_t library_key[1000];
    int records_num = 0;

    add_horizontal_rule();
    show_welcoming_message();

    // Основной цикл программы
    while (true)
    {
        // Ввод имени файла, если файл не был открыт
        if (filename[0] == '\0')
        {
            if ((rc = get_file(filename, library_init, library_key, &records_num)) != ERR_SUCCESS)
                return ERR_SUCCESS;
            add_horizontal_rule();
        }

        // Отображение меню
        show_menu();
        add_horizontal_rule();

        get_users_choice(&choice);

        if ((rc = process_choice(choice, filename, library_init, library_key, &records_num)) != ERR_SUCCESS)
            process_error(rc);
            
        puts("");
        add_horizontal_rule();
    }

    return ERR_SUCCESS;
}
