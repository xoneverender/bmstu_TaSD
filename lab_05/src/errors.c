#include "errors.h"

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
        case ERR_READING_STRING:
            puts(RED"Ошибка чтения строки."RESET);
            break;
        case ERR_ALLOCATING_MEMORY:
            puts(RED"Ошибка выделения памяти."RESET);
            break;
        case ERR_NULL_PTR:
            puts(RED"Ошибка: нулевой указатель."RESET);
            break;
        case ERR_EMPTY_QUEUE:
            puts(RED"Ошибка: стек пустой."RESET);
            break;
        case ERR_EMPTY_STRING:
            puts(RED"Ошибка: введена пустая строка."RESET);
            break;
        case ERR_QUEUE_OVERFLOW:
            puts(RED"Ошибка: переполнение стека."RESET);
    }
    return rc;
}
