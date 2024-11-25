#include "str.h"

bool is_whole_number(long *num, char *buffer)
{
    char *endptr;

    // Преобразование строки в целое число
    *num = strtol(buffer, &endptr, 10);

    // Проверка, что строка полностью преобразована в число
    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}

err_code read_string(FILE *f, char *buf, size_t len)
{
    // Чтение строки из файла
    if (!fgets(buf, len, f))
        return ERR_INVALID_FILE_FORMAT; 

    // Удаление символа новой строки
    buf[strcspn(buf, "\n")] = '\0';
    
    // Проверка, что длина строки допустима
    if (strlen(buf) > MAX_STRING_LEN)
        return ERR_STRING_TOO_LONG;

    if (strlen(buf) == 0)
        return ERR_EMPTY_STRING;

    return ERR_SUCCESS;
}

err_code validate_filename(char *filename)
{
    err_code rc = 0;

    // Проверка длины имени файла
    if (strlen(filename) > MAX_STRING_LEN)
    {
        clear_input_buffer();
        return ERR_TOO_LONG_FILENAME; 
    }

    return rc;
}

void clear_input_buffer(void)
{
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}

// Основная функция проверки
bool contains_word(const char *str, const char *word) 
{
    int word_len = strlen(word);
    int str_len = strlen(str);

    for (int i = 0; i <= str_len - word_len; i++) 
    {
        // Проверяем, совпадает ли подстрока с текущей позицией
        if (strncmp(&str[i], word, word_len) == 0) 
        {
            // Проверяем, обособлена ли подстрока пробелами или является началом/концом строки
            if ((i == 0 || str[i - 1] == ' ') && (i + word_len == str_len || str[i + word_len] == ' '))
                return true;
        }
    }
    return false;
}