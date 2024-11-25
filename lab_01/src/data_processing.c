#include "data_processing.h"

// Функция для парсинга числа из буфера и заполнения структуры number_t
int parse_number(char *buffer, number_t *num, size_t MAX_SIZE)
{
    char *delimiters = "Ee";
    char *pword;
    int dot_ind = 0;
    int rc = 0;

    // Число не должно начинаться с Е/е или точки
    if (strpbrk(buffer, "Ee") == buffer)
        return ERR_INCORRECT_STR;

    // Проверяем, сколько Ee содержит число
    if (count_el_in_string(buffer, 'e') > 1 || count_el_in_string(buffer, 'E') > 1 \
    || (count_el_in_string(buffer, 'e') > 0 && count_el_in_string(buffer, 'E') > 0))
        return ERR_INCORRECT_STR;

    // Cчитываем мантиссу числа, валидируем и присваиваем соответствующему полю структуры
    pword = strtok(buffer, delimiters);
    if ((rc = validate_mantissa(pword, MAX_SIZE)))
        return rc;
    save_mantissa(num->mantissa, &num->sign, &num->len, pword, &dot_ind);

    // Проверяем, сколько знаков +- содержит мантисса числа
    if ((count_el_in_string(pword, '+') > 0 && count_el_in_string(pword, '-') > 0) \
    || count_el_in_string(pword, '+') > 1 || count_el_in_string(pword, '-') > 1)
        return ERR_INCORRECT_STR;


    // Изменение порядка числа из-за приведения его мантиссы к целому виду
    num->exp -= dot_ind;

    // Cчитываем порядок числа, валидируем и присваиваем соответствующему полю структуры
    pword = strtok(NULL, delimiters);
    if (!pword)
        return ERR_SUCCESS;
    else if (!pword)
        return ERR_INCORRECT_STR;
    
    // Проверяем, сколько знаков +- содержит порядок числа
    if ((count_el_in_string(pword, '+') > 0 && count_el_in_string(pword, '-') > 0) \
    || count_el_in_string(pword, '+') > 1 || count_el_in_string(pword, '-') > 1)
        return ERR_INCORRECT_STR;

    if ((rc = validate_exponent(pword)))
        return rc;

    save_exponent(&num->exp, pword);
    // Проверка того, что число не содержит других частей
    pword = strtok(NULL, delimiters);
    if (pword)
        return ERR_INCORRECT_STR;

    return ERR_SUCCESS;
}

// Функция для сохранения мантиссы числа в структуру
void save_mantissa(uint8_t *mantissa, char *sign, uint8_t *len, char *string, int *dot_ind)
{
    size_t i = 0;
    char *end;

    *sign = '+';
    end = string + strlen(string) - 1;

    while (end >= string)
    {
        if (isdigit(*end))
            mantissa[i++] = *end - '0';
        else if (*end == '.')
            *dot_ind = i;
        else if (*end == '+' || *end == '-')
            *sign = *end;
        end--;
    }
    *len = i;
}

// Функция для сохранения экспоненты числа в структуру
void save_exponent(int *exp, char *str_exponent)
{
    // dot_ind уже рассчитывается справа налево (т.е. в числе 23.4 dot_ind = 1)
    *exp += atoi(str_exponent);
}

// Функция для нормализации результата
int normalize_result(number_t *res) 
{   
    size_t shift = 0;

    // Округляем число
    if (res->len > 40) 
    {
        rounding(res);
        res->len = 40;
    }
    else
    {
        // Вычисляем порядок с учетом того, что число будет представлено в формате 0.mEk
        res->exp += res->len;
    }

    // Убираем лишние нули в начале десятичной части числа
    for (size_t i = res->len; i > 0; i--)
    { 
        if (res->mantissa[i - 1] == 0)
            shift++;
        else
            break;
    }

    res->len -= shift;
    res->exp -= shift;

    // Убираем лишние нули в конце десятичной части числа
    shift = 0;
    for (size_t i = 0; i < res->len; i++)
    {
        if (res->mantissa[i] == 0)
            shift++;
        else 
            break;
    }
    for (size_t i = 0; i < res->len; i++)
        res->mantissa[i] = res->mantissa[i + shift];

    res->len -= shift;

    if (res->exp > 99999)
        return ERR_EXP_OVERFLOW;

    return ERR_SUCCESS;
}

// Функция для удаления незначащих нулей из строки
char *remove_odd_zeros(char *string)
{
    char tmp = '+';
    char *end;

    // Если число состоит из всего одного значащего нуля, оно не нуждается в обработке
    if (strlen(string) == 1 && *string == '0')
        return string;

    // Сохраняем знак числа
    if (strpbrk(string, "+-") == string)
    {
        tmp = *string;
        string++;
    }

    // Убираем нули с начала числа (00008.02 -> 8.02)
    while (isdigit(*string) && *string != '.' && *string == '0')
    {
        *string = tmp;
        string++;
    }

    // Убираем нули после десятичной точки в конце числа (20.030000 -> 20.03)
    if (strchr(string, '.'))
    {
        end = string + strlen(string) - 1;

        while (end >= string && *end != '.' && *end == '0')
            end--;

        *(end + 1) = '\0';
    }

    return string--;
}

// Результат становится +0.0e0
void zero_result(number_t *res)
{
    res->mantissa[0] = 0;
    res->sign = '+';
    res->len = 1;
    res->exp = 0;
}
