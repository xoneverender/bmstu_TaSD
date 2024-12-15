#include "errors.h"

err_code_e process_error(err_code_e rc)
{
    switch (rc)
    {
        case ERR_SUCCESS:
            break;
        case ERR_EOF_REACHED:
            puts("Выход...");
            break;
        case ERR_INVALID_STACK_INPUT:
            puts(RED"Ошибка: формат введенной строки не соответсвует ожидаемому."RESET);
            break;
        case ERR_READING_STRING:
            puts(RED"Ошибка чтения строки."RESET);
            break;
        case ERR_ALLOCATING_MEMORY:
            puts(RED"Ошибка выделения памяти."RESET);
            break;
        case ERR_NULL_PTR:
            puts(RED"Ошибка: нулевой указатель."RESET);
            break;
        case ERR_EMPTY_STACK:
            puts(RED"Ошибка: стек пустой."RESET);
            break;
        case ERR_EMPTY_STRING:
            puts(RED"Ошибка: введена пустая строка."RESET);
            break;
        case ERR_STACK_OVERFLOW:
            puts(RED"Ошибка: переполнение стека."RESET);
            break;
        case ERR_CALCULATIONS:
            puts(RED"Ошибка во время выполнения вычислений. Убедитесь, что выражение, лежащее на стеке, корректно."RESET);
            break;
        case ERR_DIVISION_BY_ZERO:
            puts(RED"Ошибка: попытка произвести деление на 0."RESET);
            break;
        case ERR_INVALID_EXPRESSION_LEN:
            puts(RED "Ошибка: получена некорректная длина выражения." RESET);
            break;
        case ERR_OPENING_FILE:
            puts(RED "Ошибка: файл не может быть открыт." RESET);
            break;
        case ERR_INVALID_DATA_IN_FILE:
            puts(RED "Ошибка: в файле содержатся некорректные данные." RESET);
            break;
    }
    return rc;
}
