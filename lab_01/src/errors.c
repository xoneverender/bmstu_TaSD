#include "errors.h"

void process_error(size_t err_code)
{
    switch (err_code)
    {
        case ERR_EMPTY_LINE:
            puts("Ошибка: Пустая строка.");
            break;
        case ERR_INCORRECT_STR:
            puts("Ошибка: Формат введенной строки не соответствует ожидаемому.");
            break;
        case ERR_MANTISSA_TOO_LONG:
            puts("Ошибка: Мантисса числа превышает допустимую длину.");
            break;
        case ERR_EXP_TOO_LONG:
            puts("Ошибка: Порядок числа превышает допустимую длину.");
            break;
        case ERR_NOT_A_REAL_NUM:
            puts("Ошибка: Мантисса должна быть действительным числом.");
            break;
        case ERR_NOT_A_WHOLE_NUM:
            puts("Ошибка: Порядок числа должен быть целым числом.");
            break;
        case ERR_EXP_OVERFLOW:
            puts("Ошибка: Произошло переполнение порядка произведения.");
            break;
    }
    exit(err_code);
}
