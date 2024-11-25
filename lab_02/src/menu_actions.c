#include "menu_actions.h"

err_code process_choice(long choice, char *filename, book_t *records, key_t *keys, int *records_num)
{
    err_code rc = ERR_SUCCESS;

    switch (choice)
    {
        case 0:
            puts("Выход...");
            exit(0);    
        case 1:
            // Логика для добавления записи
            if ((rc = add_record(records, records_num)) == ERR_SUCCESS)
            {
                create_keys_arr(keys, records, *records_num);
                puts("\nЗапись успешно добавлена");
            }
            break;
        case 2:
            // Логика для удаления записи
            if ((rc = delete_record(records, records_num)) == ERR_SUCCESS)
            {    
                create_keys_arr(keys, records, *records_num);
                puts("\nЗапись успешно удалена");
            }
            break;
        case 3:
            // Логика сортировки исходной таблицы
            bubblesort(records, *records_num, sizeof(book_t), compare_books);
            // bubblesort_books_arr(records, *records_num);

            // По идее если исходная таблица и таблица с ключами связаны (index в массиве ключей - индекс записи с этой фамилией в исходной таблице), то массив ключей
            // сортируется вместе с исходной таблицей 
            create_keys_arr(keys, records, *records_num);

            puts("Исходная таблица успешно отсортированна");
            break;
        case 4:
            // Логика сортировки таблицы ключей
            bubblesort(keys, *records_num, sizeof(key_t), compare_keys);
            // bubblesort_keys_arr(keys, *records_num);
            puts("Таблица ключей успешно отсортированна");
            break;
        case 5:
            // Логика просмотра исходной таблицы
            print_book_arr(records, *records_num);
            break;
        case 6:
            // Логика просмотра таблицы ключей
            print_key_arr(keys, *records_num);
            break;
        case 7:
            // Логика просмотра исходной таблицы в упорядоченном виде, используя упорядоченную таблицу ключей
            print_table_using_keys(records, keys, *records_num);
            break;
        case 8:
            // Поиск всех романов автора
            if ((rc = find_record(records, *records_num)) == ERR_SUCCESS)
                puts("Найдены все произведения авторов с введенной фамилией.");
            break;
        case 9:
            // Перемешивание элементов исходной таблицы
            // shuffle_books(records, *records_num);
            shuffle(records, *records_num, sizeof(book_t));
            create_keys_arr(keys, records, *records_num);
            puts("Исходная таблица успешно перемешана");
            break;
        case 10:
            // Сброс имени файла для выбора нового
            // filename[0] = '\0';
            rc = get_file(filename, records, keys, records_num);
            puts("");
            break;
        case 11:
            // Логика сравнения сортировок
            compare_sorts(records, keys, *records_num);
            break;  
        case 12:
            // Логика для сохранения записей
            if ((rc = save_in_file(records, *records_num, filename)) == ERR_SUCCESS)
                puts("\nЗаписи успешно сохранены в файл");
            else
                puts("\nЗаписи не были сохранены:(");
            break;
        default:
            puts("Некорректный выбор, попробуйте снова.");
    }

    return rc;
}

err_code add_record(book_t *records, int *records_num)
{
    err_code rc = ERR_SUCCESS;
    literature_type_t lit_type;
    fiction_t fic_type;
    children_genre_t children_genre;
    char buf[MAX_STRING_LEN + 2];
    long temp_value;

    if (*records_num == 1000)
        return ERR_ADDING_RECORD;

    puts("");
    add_horizontal_rule();
    puts("\nДОБАВЛЕНИЕ ЗАПИСИ");
    
    // Чтение и проверка строки с именем автора
    while (true)
    {
        puts("\nВведите имя автора: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);
        else
        {
            strcpy(records[*records_num].author, buf);
            printf("Записано имя: %s\n\n", records[*records_num].author);
            break;
        }
    }

    // Чтение и проверка строки с названием книги
    while (true)
    {
        puts("\nВведите название произведения: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);
        else
        {
            strcpy(records[*records_num].title, buf);
            printf("Записано название книги: %s\n\n", records[*records_num].title);
            break;
        }
    }

    // Чтение и проверка строки с названием издательства
    while (true)
    {
        puts("\nВведите название издательства: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);
        else
        {
            strcpy(records[*records_num].publisher, buf);
            printf("Записано название издательства: %s\n\n", records[*records_num].publisher);
            break;
        }
    }

    // Чтение и проверка количества страниц 
    while (true)
    {
        puts("\nВведите количество страниц: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }

        if (is_whole_number(&temp_value, buf) && temp_value < USHRT_MAX && temp_value > 0)
        {
            records[*records_num].pages = (uint16_t)temp_value;
            printf("Записано количество страниц: %hu\n\n", records[*records_num].pages);
            break;
        }
        else if (temp_value > USHRT_MAX)
            puts("Введено слишком большой число страниц. Повторите ввод.\n");
        else
            puts("Число страниц должно быть целым положительным числом. Повторите попытку\n");
    }

    while (true)
    {
        puts("\nВведите 1, если это техническая литература,");
        puts("        2, если художественная,");
        puts("        3, если детская: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }

        if (is_whole_number(&temp_value, buf) && temp_value >= 0 && temp_value <= 3)
        {
            lit_type = temp_value - 1;
            records[*records_num].type = lit_type;
            break;
        }
        else
            puts("Должно быть введено целое число от 1 до 3. Повторите попытку\n");
    }

    switch (lit_type)
    {
        case TECHNICAL:
        {
            puts("Выбранный жанр: техническая литература");

            // Чтение и проверка строки с отраслью технической литературы
            while (true)
            {
                puts("\nВведите отрасль технической литературы: ");
                add_input_arrow();

                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                    process_error(rc);
                else
                {
                    strcpy(records[*records_num].type_t.technical.branch, buf);
                    printf("Записана отрасль: %s\n\n", records[*records_num].type_t.technical.branch);
                    break;
                }
            }

            while (true)
            {
                puts("\nВведите 1, если это переведенная литература, или 2, если отечественная: ");
                add_input_arrow();

                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                {
                    process_error(rc);
                    continue;
                }

                if (is_whole_number(&temp_value, buf) && temp_value >= 1 && temp_value <= 2)
                {                
                    if (temp_value == 1)
                    {
                        records[*records_num].type_t.technical.is_translated = true;
                        printf("Ваш выбор: переведенная литература\n");
                    }
                    else
                    {
                        records[*records_num].type_t.technical.is_translated = false;
                        printf("Ваш выбор: отчественная литература\n");
                    }                        
                    break;
                }
                else
                    puts("Должно быть введено целое число от 1 до 2. Повторите попытку\n");
            }
            
            // Чтение и проверка года публикации
            while (true)
            {
                puts("\nВведите год публикации: ");
                add_input_arrow();

                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                {
                    process_error(rc);
                    continue;
                }

                if (is_whole_number(&temp_value, buf) && temp_value >= 0 && temp_value <= 2024)
                {
                    records[*records_num].type_t.technical.year_of_publication = (uint16_t)temp_value;
                    printf("Записан год публикации: %hu\n\n", records[*records_num].type_t.technical.year_of_publication);
                    break;
                }
                else
                    puts("Год публикации должен быть целым числом от 0 до 2024. Повторите попытку\n");
            }

            break;
        }
        case FICTION:
        {
            puts("Выбранный жанр: художественная литература");

            while (true)
            {
                puts("\nВведите 1, если это роман,");
                puts("        2, если пьеса,");
                puts("        3, если поэзия: ");
                add_input_arrow();
                
                
                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                {
                    process_error(rc);
                    continue;
                }

                if (is_whole_number(&temp_value, buf) && temp_value > 0 && temp_value <= 3)
                {
                    fic_type = temp_value - 1;
                    records[*records_num].type_t.fiction = fic_type;

                    if (fic_type == NOVEL)
                        printf("Ваш выбор: роман\n");
                    else if (fic_type == PLAY)
                        printf("Ваш выбор: пьеса\n");
                    else
                        printf("Ваш выбор: поэзия\n");
                    break;
                }
                else
                    puts("Должно быть введено целое число от 1 до 3. Повторите попытку\n");
            }

            break;
        }
        case CHILDREN:
        {
            puts("Выбранный жанр: детская литература");

            // Чтение и проверка минимального возраста 
            while (true)
            {
                puts("\nВведите минимальный возрат: ");
                add_input_arrow();

                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                {
                    process_error(rc);
                    continue;
                }

                if (is_whole_number(&temp_value, buf) && temp_value >= 0 && temp_value < 18)
                {
                    records[*records_num].type_t.children.min_age = (uint16_t)temp_value;
                    printf("Записан возраст: %hu\n\n", records[*records_num].type_t.children.min_age);
                    break;
                }
                else
                    puts("Минимальный возраст должен быть целым числом от 0 до 17. Повторите попытку\n");
            }

            while (true)
            {
                puts("\nВведите 1, если это стихи, или 2, если это сказки: ");
                add_input_arrow();

                if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                {
                    process_error(rc);
                    continue;
                }

                if (is_whole_number(&temp_value, buf) && temp_value >= 0 && temp_value <= 2)
                {
                    children_genre = temp_value - 1;
                    records[*records_num].type_t.children.genre = children_genre;
                    if (children_genre == POEMS)
                        printf("Ваш выбор: роман\n");
                    else
                        printf("Ваш выбор: поэзия\n");
                    break;
                }
                else
                    puts("Должно быть введено целое число от 1 до 2. Повторите попытку\n");
            }
            break;
        }
    }

    *records_num += 1;
    
    puts("");

    return ERR_SUCCESS;
}

err_code delete_record(book_t *records, int *records_num)
{
    err_code rc = ERR_SUCCESS;
    char buf[MAX_STRING_LEN + 2];
    int pos = -1;
    size_t counter = 1;

    if (*records_num == 40)
        return ERR_DELITING_RECORD;

    puts("");
    add_horizontal_rule();
    puts("\nУДАЛЕНИЕ ЗАПИСИ");

    while (true)
    {   
        puts("\nВведите название книги, запись для которой вы хотите удалить: ");
        add_input_arrow();
        // Чтение и проверка названия книги, запись для которой должна быть удалена

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);
        else
        {
            printf("Ваш выбор: %s\n", buf);

            // Поиск позиции, на которой находится запись
            for (size_t i = 0; i < (size_t)*records_num; i++)
            {
                if (strcasecmp(records[i].title, buf) == 0)
                {
                    pos = i;
                    break;
                }
            }

            // Если запись была найдена
            if (pos >= 0)
            {
                for (size_t i = pos; i < (size_t)*records_num; i++)
                    records[i] = records[i + 1];
                break;
            }
            else
                puts("Книги с подобным названием не было найдено.");
        }

        if (counter == 5)
        {
            counter = 0;
            if ((rc = ask_to_proceed()) != ERR_SUCCESS)
                return rc;
        }
        counter++;
    }

    *records_num -= 1;

    puts("");

    return ERR_SUCCESS;
}

err_code find_record(book_t *records, int records_num)
{
    err_code rc = ERR_SUCCESS;
    char buf[MAX_STRING_LEN + 2];
    bool novels_found = false;
    size_t counter = 1;

    puts("");
    add_horizontal_rule();
    puts("\nПОИСК ВСЕХ РОМАНОВ АВТОРА");
    puts("");

    while (true)
    {
        puts("\nВведите фамилию автора, романы которого вы хотите найти: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
            process_error(rc);
        else
        {
            printf("Ваш выбор: %s\n", buf);

            for (size_t i = 0; i < (size_t)records_num; i++)
            {
                if (contains_word(records[i].author, buf) && records[i].type_t.fiction == NOVEL)
                {
                    novels_found = true;
                    print_book(records[i]);
                }
            }
            if (novels_found)
                break;
            else
                puts("Автор, писавший романы, с таким именем не был найден.");                
        }

        if (counter == 5)
        {
            counter = 1;
            if ((rc = ask_to_proceed()) != ERR_SUCCESS)
                return rc;
        }
        counter++;
    }

    return ERR_SUCCESS;
}

unsigned long long count_sorting_time(void *base, size_t elems_num, size_t elems_size, int (*compare)(const void *, const void *), void (*sort)(void *, size_t, size_t, int (*)(const void *, const void *)))
{
    struct timeval start, end;
    unsigned long long start_us, end_us;

    shuffle(base, elems_num, elems_size);
    gettimeofday(&start, NULL);
    sort(base, elems_num, elems_size, compare);
    gettimeofday(&end, NULL);

    start_us = (unsigned long long)start.tv_sec * 1000000ULL + start.tv_usec;
    end_us = (unsigned long long)end.tv_sec * 1000000ULL + end.tv_usec;

    return end_us - start_us;
}

void compare_sorts(book_t *records, key_t *keys, int records_num)
{
    size_t measures_num = 1000;
    long long records_bubblesort_time = 0, keys_bubblesort_time = 0, records_qsort_time = 0, keys_qsort_time = 0;
    long long bubblesort_time_gain = 0, qsort_time_gain = 0;
    double ratio = 0;

    for (size_t i = 0; i < measures_num; i++)
    {
        records_bubblesort_time += count_sorting_time(records, records_num, sizeof(book_t), compare_books, bubblesort);
        keys_bubblesort_time += count_sorting_time(keys, records_num, sizeof(key_t), compare_keys, bubblesort);
        records_qsort_time += count_sorting_time(records, records_num, sizeof(book_t), compare_books, qsort);
        keys_qsort_time += count_sorting_time(keys, records_num, sizeof(key_t), compare_keys, qsort);
    }
    
    records_bubblesort_time /= measures_num;
    keys_bubblesort_time /= measures_num;
    records_qsort_time /= measures_num;
    keys_qsort_time /= measures_num;

    bubblesort_time_gain = (records_bubblesort_time != keys_bubblesort_time) ? (double)(records_bubblesort_time - keys_bubblesort_time) / records_bubblesort_time * 100 : 0;
    qsort_time_gain = (records_qsort_time != keys_qsort_time) ? (double)(llabs(records_qsort_time - keys_qsort_time)) / records_qsort_time * 100 : 0;
    ratio = (double)(sizeof(key_t) + sizeof(book_t)) / sizeof(book_t);
    
    puts("");
    add_horizontal_rule();
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    printf("|%24s|%-48s|%-46s|%-44s|\n", "", "     Исходная таблица, мкс", "     Таблица ключей, мкс", "      Прирост времени, %");
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    add_horizontal_rule();
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    printf("|%-43s|  %-27llu |  %-28llu|  %-28llu|\n", "  Сортировка пузырьком", records_bubblesort_time, keys_bubblesort_time, bubblesort_time_gain);
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    add_horizontal_rule();
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    printf("|%-41s|  %-27llu |  %-28llu|  %-28llu|\n", "   Быстрая сортировка", records_qsort_time, keys_qsort_time, qsort_time_gain);
    printf("|%24s|%30s|%30s|%30s|\n", "", "", "", "");
    add_horizontal_rule();

    printf("\n\n");

    add_horizontal_rule();
    printf("|%37s|%43s|%35s|\n", "", "", "");
    printf("|%-55s|%-74s|%-45s|\n", "        Таблица записей, байт", "  Таблица записей + таблица ключей, байт", "           Разница, раз");
    printf("|%37s|%43s|%35s|\n", "", "", "");
    add_horizontal_rule();
    printf("|%37s|%43s|%35s|\n", "", "", "");
    printf("|  %-35ld|  %-41ld|  %-33.6f|\n", sizeof(book_t), sizeof(key_t) + sizeof(book_t), ratio);
    printf("|%37s|%43s|%35s|\n", "", "", "");
    add_horizontal_rule();
}
