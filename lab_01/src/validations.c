#include "validations.h"

// Функция для валидации мантиссы
int validate_mantissa(char *string, size_t MAX_SIZE)
{
    if (string == NULL)
       return ERR_EMPTY_LINE;

    string = remove_odd_zeros(string);

    if (count_digits_num(string) > (int)MAX_SIZE)
        return ERR_MANTISSA_TOO_LONG;

    if (isspace(*string) || isspace(*(string + strlen(string) - 1)))
        return ERR_INCORRECT_STR;

    if (!is_real_number(string))
        return ERR_NOT_A_REAL_NUM;

    return ERR_SUCCESS;
}

// Функция для валидации экспоненты
int validate_exponent(char *string)
{
    string = remove_odd_zeros(string);

    if (count_digits_num(string) > MAX_EXP_SIZE)
        return ERR_EXP_TOO_LONG;

    if (isspace(*(string + strlen(string) - 1)))
        return ERR_INCORRECT_STR;

    if (!is_whole_number(string))
        return ERR_NOT_A_WHOLE_NUM;

    return ERR_SUCCESS;
}

// Функция для проверки, является ли строка действительным числом
bool is_real_number(char *string)
{
    char *endptr;

    (void)strtod(string, &endptr);
    if (endptr == string || *endptr != '\0')
        return false;

    return true;
}

// Функция для проверки, является ли строка целым числом
bool is_whole_number(char *string)
{
    char *endptr;

    (void)strtol(string, &endptr, 10);
    if (endptr == string || *endptr != '\0')
        return false;
    return true;
}

// Функция для проверки, является ли множитель нулем
bool is_null(number_t *num)
{
    if (num->mantissa[0] == 0 && num->len == 1)
        return true;
    return false;
}
