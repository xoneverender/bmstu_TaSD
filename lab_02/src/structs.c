#include "structs.h"

err_code read_struct(FILE *f, char *buf, book_t *record)
{
    err_code rc = ERR_SUCCESS;
    literature_type_t lit_type;
    long temp_value;  // Временная переменная для проверки знаковых чисел

    if (!fgets(buf, 5, f))
        return ERR_INVALID_FILE_FORMAT;

    buf[strcspn(buf, "\n")] = '\0';

    if (strcmp(buf, "---") != 0)
        return ERR_INVALID_FILE_FORMAT;

    // Чтение и проверка строки с именем автора
    if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        return rc;
    strcpy(record->author, buf);

    // printf("author: %s, ", buf);
    // Чтение и проверка строки с названием книги
    if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        return rc;
    strcpy(record->title, buf);

    // printf("title: %s\n", buf);
    // Чтение и проверка строки с названием издательства
    if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        return rc;
    strncpy(record->publisher, buf, MAX_STRING_LEN);

    // Чтение и проверка количества страниц
    if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        return rc;
    if (!is_whole_number(&temp_value, buf) || temp_value <= 0)
        return ERR_INVALID_FILE_FORMAT;
    record->pages = (uint16_t)temp_value;

    // Чтение и проверка типа литературы
    if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
        return rc;
    if (!is_literature_type(buf, &lit_type))
        return ERR_INVALID_FILE_FORMAT;
    record->type = lit_type;

    // Чтение данных в зависимости от типа литературы
    switch (lit_type)
    {
        case TECHNICAL:
            // Чтение и проверка строки с отраслью технической литературы
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            strncpy(record->type_t.technical.branch, buf, MAX_STRING_LEN);

            // Чтение и проверка строки, содержащей true/false
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            if (!is_boolean(buf, &record->type_t.technical.is_translated))
                return ERR_INVALID_FILE_FORMAT;

            // Чтение и проверка года публикации
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            if (!is_whole_number(&temp_value, buf) || temp_value < 0)
                return ERR_INVALID_FILE_FORMAT;
            record->type_t.technical.year_of_publication = (uint16_t)temp_value;
            break;

        case FICTION:
            // Чтение и проверка строки, содержащей тип художественной литературы
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            if (!is_fiction_type(buf, &record->type_t.fiction))
                return ERR_INVALID_FILE_FORMAT;
            break;

        case CHILDREN:
            // Чтение и проверка минимального возраста (знаковое -> беззнаковое)
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            if (!is_whole_number(&temp_value, buf) || temp_value < 0)
                return ERR_INVALID_FILE_FORMAT;
            record->type_t.children.min_age = (uint16_t)temp_value;

            // Чтение и проверка строки, содержащей тип детской литературы
            if ((rc = read_string(f, buf, MAX_STRING_LEN + 2)) != ERR_SUCCESS)
                return rc;
            if (!is_children_type(buf, &record->type_t.children.genre))
                return ERR_INVALID_FILE_FORMAT;
            break;

        default:
            return ERR_INVALID_FILE_FORMAT;
    }

    return ERR_SUCCESS;
}

void create_keys_arr(key_t *keys, book_t *records, const int records_num)
{
    for (uint16_t i = 0; i < records_num; i++)
    {   
        keys[i].index = i;
        strcpy(keys[i].title, records[i].title);
    } 
}

bool is_literature_type(const char *buf, literature_type_t *type)
{
    if (strcmp(buf, "TECHNICAL") == 0) 
        *type = TECHNICAL;
    else if (strcmp(buf, "FICTION") == 0)
        *type = FICTION;
    else if (strcmp(buf, "CHILDREN") == 0)
        *type = CHILDREN;
    else
        return false;

    return true;
}

bool is_fiction_type(const char *buf, fiction_t *type)
{
    if (strcmp(buf, "NOVEL") == 0) 
        *type = NOVEL;
    else if (strcmp(buf, "PLAY") == 0)
        *type = PLAY;
    else if (strcmp(buf, "POETRY") == 0)
        *type = POETRY;
    else
        return false;

    return true;
}

bool is_children_type(const char *buf, children_genre_t *type)
{
    if (strcmp(buf, "POEMS") == 0) 
        *type = POEMS;
    else if (strcmp(buf, "FAIRY_TALES") == 0)
        *type = FAIRY_TALES;
    else
        return false;

    return true;
}

bool is_boolean(const char *buf, bool *boolean)
{
    if (strcmp(buf, "true") == 0)
        *boolean = true;
    else if (strcmp(buf, "false") == 0)
        *boolean = false;
    else
        return false;

    return true;
}

void bubblesort(void *base, size_t num, size_t elems_size, int (*compare)(const void *, const void *))
{
    char *arr = base; // Приводим к типу char* для работы с байтовым смещением
    void *a, *b;

    for (size_t j = num - 1; j > 0; j--)
    {
        for (size_t i = 0; i < j; i++)
        {
            // Указатели на соседние элементы для сравнения и обмена
            a = arr + i * elems_size;
            b = arr + (i + 1) * elems_size;

            // Сравнение элементов и обмен, если необходимо
            if (compare(a, b) > 0)
                swap(a, b, elems_size);
        }
    }
}

void swap(void *a, void *b, size_t elems_size)
{
    char temp;
    char *x = a;
    char *y = b;

    for (size_t i = 0; i < elems_size; i++)
    {
        temp = x[i];
        x[i] = y[i];
        y[i] = temp;
    }
}

int compare_books(const void *book1, const void *book2)
{
    return strcasecmp(((book_t*)book1)->title, ((book_t*)book2)->title);
}

int compare_keys(const void *key1, const void *key2)
{
    return strcasecmp(((key_t*)key1)->title, ((key_t*)key2)->title);
}

void shuffle(void *array, const int elems_num, const size_t elems_size)
{
    size_t j = 0;
    char *arr = array;

    // Инициализация генератора случайных чисел
    srand((unsigned int)time(NULL));

    for (size_t i = elems_num - 1; i > 0; i--)
    {
        // Выбор случайного индекса от 0 до i
        j = rand() % (i + 1);

        // Обмен элементов по индексу i и j
        swap(arr + i * elems_size, arr + j * elems_size, elems_size);
    }
}
