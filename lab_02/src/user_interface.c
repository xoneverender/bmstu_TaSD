#include "user_interface.h"

void show_welcoming_message(void)
{
    puts("");
    puts("ОПИСАНИЕ ПРОГРАММЫ:");
    puts("");
    printf("Данная программа принимает на вход файл, содержащий таблицу не менее чем\n");
    printf("с 40 и не более чем с 1000 записей, каждая из которых представляет собой:\n\
    - фамилию автора;\n\
    - название книги;\n\
    - издательство;\n\
    - количество страниц;\n\
    - вид литературы (техническая, художественная, детская);\n\
    - детали, относящиеся к выбранному виду литературы:\n\
    для технической: отрасль, отечественная/переводная, год издания;\n\
    для художественной: тип литературы (роман, пьеса, поэзия);\n\
    для детской: минимальный возраст, тип литературы (стихи, сказки).\n\
    Длина каждого поля должна быть не больше %d символов латиницей (%d символов кириллицей)\n\n", MAX_STRING_LEN, MAX_STRING_LEN / 2);
}

void show_menu(void)
{
    // Отображение меню
    puts("");
    puts("МЕНЮ:");
    puts("");
    puts("1. Добавление записи"); 
    puts("2. Удаление записи по названию книги"); 
    puts("3. Сортировка исходной таблицы");
    puts("4. Сортировка таблицы ключей");
    puts("5. Просмотр исходной таблицы");
    puts("6. Просмотр таблицы ключей");
    puts("7. Просмотр исходной таблицы, используя ключи");
    puts("8. Поиск всех романов автора");
    puts("9. Перемешивание элементов исходной таблицы");
    puts("10. Выбор нового файла");
    puts("11. Сравнение сортировок");
    puts("12. Сохранение измененной таблицы в файл");
    puts("0. Выход");
    puts("");
}

err_code get_file(char *filename, book_t *records, key_t *keys, int *records_num)
{
    err_code rc = ERR_SUCCESS;
    size_t counter = 1;

    puts("");
    add_horizontal_rule();
    puts("\nВЫБОР ФАЙЛА");

    while (true)
    {   
        puts("\nВведите название файла с таблицей: ");
        add_input_arrow();

        if (fgets(filename, MAX_STRING_LEN + 2, stdin))
        {
            // Удаление символа новой строки
            filename[strcspn(filename, "\n")] = '\0';

            // Проверка имени файла
            if ((rc = validate_filename(filename)) != ERR_SUCCESS)
                process_error(rc);
            // Проверка открытия файла
            else if ((rc = open_file(filename)) != ERR_SUCCESS)
                process_error(rc);
            // Чтение файла в массив структур
            else if ((rc = read_file(filename, records, keys, records_num)) != ERR_SUCCESS)
                process_error(rc);

            if (rc == ERR_SUCCESS)
            {
                printf("Файл успешно открыт и прочитан: %s\n\n", filename);
                return ERR_SUCCESS;
            }
        }

        // Если пользователь за пять попыток не смог ввести корректное название файла - спрашиваем о необходимости продолжать попытки
        if (counter == 5)
        {
            // Вызов функции для запроса продолжения или выхода
            rc = confirm_file_retry();
            if (rc != ERR_SUCCESS)
                return rc;  // Пользователь выбрал выход
            counter = 0;  // Сбрасываем счётчик попыток
        }
        counter++;  // Увеличиваем счётчик попыток
    }
}

err_code confirm_file_retry(void)
{
    long choice = 0;
    char buffer[MAX_STRING_LEN + 2];
    err_code rc = ERR_SUCCESS;

    while (true)
    {   
        puts("\nЕсли желаете продолжить выбор файла, нажмите 1, если выйти - любое другое число.");
        add_input_arrow();

        
        if ((rc = read_string(stdin, buffer, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }

        if (is_whole_number(&choice, buffer))
        {
            if (choice == 1)
                return ERR_SUCCESS;  // Продолжаем выбор файла
            else
                return ERR_FILENAME_NOT_PROVIDED;  // Пользователь выбрал выход
        }

        puts("Некорректный ввод, попробуйте снова.");
    }
}

void get_users_choice(long *choice)
{
    char buffer[MAX_STRING_LEN + 2];
    err_code rc = ERR_SUCCESS;

    while (true)
    {
        puts("\nВыберите действие (от 0 до 12): ");
        add_input_arrow();


        if ((rc = read_string(stdin, buffer, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }

        if (is_whole_number(choice, buffer))
        {
            if (*choice >= 0 && *choice <= 12)
                return;
            else
                puts("Выбранный пункт меню должен быть целым числом от 0 до 12. Повторите ввод.");
        }
        else
            puts("Введено нечисленное значение. Попробуйте снова.");
    }
}

void add_horizontal_rule(void)
{
    puts("-----------------------------------------------------------------------------------------------------------------------");
}

void add_input_arrow(void)
{
    printf(">>>>>> ");
}

void print_book_arr(const book_t *records, const int records_num)
{
    puts("");
    add_horizontal_rule();
    puts("\nПРОСМОТР ИСХОДНОЙ ТАБЛИЦЫ\n");

    for (int i = 0; i < records_num; i++)
    {   
        printf("%d) ", i + 1);
        print_book(records[i]);
    }
    puts("");
}

void print_book(const book_t record)
{
    printf("%s; ", record.author);
    printf(BOLD_TEXT "%s; " RESET_COLOR, record.title);
    printf("%s; ", record.publisher);
    printf("%hu; ", record.pages);

    if (record.type == TECHNICAL)
    {
        printf("%s; ", "Техническая");
        printf("%s; ", record.type_t.technical.branch);
        if (record.type_t.technical.is_translated == true)
            printf("Переводная; ");
        else
            printf("Отчественная; ");
        printf("%hu", record.type_t.technical.year_of_publication);
    }
    else if (record.type == CHILDREN)
    {
        printf("%s; ", "Детская");
        printf("%hu; ", record.type_t.children.min_age);
        if (record.type_t.children.genre == POEMS)
            printf("%s", "Стихи");
        else
            printf("%s", "Сказки");
    }
    else
    {
        printf("%s; ", "Художественная");
        if (record.type_t.fiction == NOVEL)
            printf("%s", "Роман");
        else if (record.type_t.fiction == NOVEL)
            printf("%s", "Пьеса");
        else
            printf("%s", "Поэзия");
    }
    puts("");
}

void print_key_arr(const key_t *keys, const int keys_num)
{
    puts("");
    add_horizontal_rule();
    puts("\nПРОСМОТР ТАБЛИЦЫ КЛЮЧЕЙ\n");

    for (uint16_t i = 0; i < keys_num; i++)
    {   
        printf("%hu: ", keys[i].index);
        printf("%s\n", keys[i].title);
    } 
    puts("");
}

void print_table_using_keys(const book_t *records, const key_t *keys, const int records_num)
{
    puts("");
    add_horizontal_rule();
    puts("\nПРОСМОТР ИСХОДНОЙ ТАБЛИЦЫ С ПОМОЩЬЮ ТАБЛИЦЫ КЛЮЧЕЙ\n");

    for (uint16_t i = 0; i < records_num; i++)
    {
        printf("%d) ", i + 1);
        print_book(records[keys[i].index]);
    }
    puts("");
}

err_code ask_to_proceed(void)
{
    char buf[MAX_STRING_LEN + 2];
    long choice;
    err_code rc = ERR_SUCCESS;

    while (true)
    {
        puts("\nЖелаете продолжить? (1 - да, любое другое число - нет)");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);

        if (is_whole_number(&choice, buf))
        {
            if (choice == 1)
                break;
            else
                return ERR_NOT_DELETED;
        }
        else
            puts("Должно быть введено целое число. Повторите попытку");
    }
    return ERR_SUCCESS;
}
