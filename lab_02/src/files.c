#include "files.h"

err_code open_file(char *filename)
{
    FILE *f = fopen(filename, "r");

    if (f == NULL)
        return ERR_OPENING_FILE;

    fclose(f);

    return ERR_SUCCESS;
}

err_code read_records_num(FILE *f, int *records_num)
{
    char buffer[10];
    
    *records_num = 0;

    if (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (!is_whole_number((long *)records_num, buffer))
            return ERR_INVALID_RECORDS_NUM;
    }
    else
        return ERR_EMPTY_FILE;
    
    return ERR_SUCCESS;
}

err_code read_file(char *filename, book_t *records, key_t *keys, int *records_num)
{
    // title_name - наибольшая длина среди всех полей
    char buffer[MAX_STRING_LEN + 3];
    int rc = ERR_SUCCESS;
    
    FILE *f = fopen(filename, "r");
    if (f == NULL)
        return ERR_OPENING_FILE;

    // Проверка того, что количество записей описано в файле
    if ((rc = read_records_num(f, records_num)) != ERR_SUCCESS)
    {
        fclose(f);
        return rc;
    }

    // Проверка того, что количество записей входит в допустимый диапазон
    if (*records_num < 40 || *records_num > 1000)
    {
        fclose(f);
        return ERR_INVALID_RECORDS_NUM;
    }

    // Чтение каждой структуры из файла в массив
    for (uint16_t i = 0; i < (uint16_t)*records_num; i++)
    {
        // printf("i = %hu\n", i + 1);
        rc = read_struct(f, buffer, &records[i]);
        if (rc)
        {
            fclose(f);
            return ERR_INVALID_FILE_FORMAT;
        }
    }

    create_keys_arr(keys, records, *records_num);

    // Если после чтения всех записей файл не пуст, то ошибка формата
    if (fgets(buffer, sizeof(buffer), f) != NULL)
    {
        fclose(f);
        return ERR_INVALID_FILE_FORMAT;
    }

    fclose(f);

    return ERR_SUCCESS;
}

err_code save_in_file(book_t *records, int records_num, char *filename)
{
    err_code rc = ERR_SUCCESS;
    FILE *outfile;
    char buf[MAX_STRING_LEN + 2];
    long temp_value = 0;
    bool save_in_infile = true;
    size_t counter = 1;

    puts("");
    add_horizontal_rule();
    puts("");
    puts("СОХРАНЕНИЕ ИЗМЕНЕННОЙ ТАБЛИЦЫ В ФАЙЛ");
    puts("");

    while (true)
    {
        puts("\nВведите 1, если хотите сохранить изменения в исходном файле, или 2, если в новом: ");
        add_input_arrow();

        if ((rc = read_string(stdin, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        {
            process_error(rc);
            continue;
        }

        if (is_whole_number(&temp_value, buf) && temp_value >= 1 && temp_value <= 2)
        {                
            if (temp_value == 1)
                printf("Ваш выбор: сохранить в исходном файле\n");
            else
            {
                printf("Ваш выбор: сохранить в новом файле\n");
                save_in_infile = false;           
            }
            break;
        }
        else
            puts("Должно быть введено целое число от 1 до 2. Повторите попытку\n");
    }

    if (save_in_infile)
    {
        outfile = fopen(filename, "w");
        if (outfile == NULL)
            process_error(ERR_OPENING_FILE);
    }
    else
    {
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
                outfile = fopen(filename, "w");
                if (outfile == NULL)
                    process_error(ERR_OPENING_FILE);
            }
            else
                puts("Строка не была прочитана. Повторите ввод."); 


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

    // Записываем количество записей в начало файла
    fprintf(outfile, "%d\n", records_num);

    for (int i = 0; i < records_num; i++)
    {   
        // Разделитель между записями книг
        fprintf(outfile, "---\n");

        // Запись общих данных книги
        fprintf(outfile, "%s\n%s\n%s\n%u\n", records[i].author, records[i].title, records[i].publisher, records[i].pages);

        // Определяем тип литературы и записываем соответствующие данные
        switch (records[i].type) 
        {
            case TECHNICAL:
                fprintf(outfile, "TECHNICAL\n");
                fprintf(outfile, "%s\n%s\n%u\n", records[i].type_t.technical.branch,
                        records[i].type_t.technical.is_translated ? "true" : "false",
                        records[i].type_t.technical.year_of_publication);
                break;
            case FICTION:
                fprintf(outfile, "FICTION\n");
                if (records[i].type_t.fiction == NOVEL)
                    fprintf(outfile, "%s\n", "NOVEL");
                else if (records[i].type_t.fiction == PLAY)
                    fprintf(outfile, "%s\n", "PLAY");
                else 
                    fprintf(outfile, "%s\n", "POETRY");
                break;
            case CHILDREN:
                fprintf(outfile, "CHILDREN\n");
                fprintf(outfile, "%u\n", records[i].type_t.children.min_age);
                fprintf(outfile, "%s\n", records[i].type_t.children.genre == POEMS ? "POEMS" : "FAIRY_TALES");
                break;
        }
    }
    
    fclose(outfile);

    return ERR_SUCCESS;
}
