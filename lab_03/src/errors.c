#include "errors.h"

err_code process_error(err_code error)
{
    switch(error)
    {
        case ERR_OPENING_FILE:
            puts(RED "Ошибка: Данный файл не может быть открыт или его не существует." RESET);
            break;
        case ERR_ALLOCATING_MEMORY:
            puts(RED "Ошибка выделения памяти." RESET);
            break;
        case ERR_EMPTY_FILE:
            puts(RED "Ошибка: Файл пустой." RESET);
            break;
        case ERR_INVALID_FILE_FORMAT:
            puts(RED "Ошибка: Формат этого файла не соответствует формату, с которым работает программа." RESET);
            break;
        case ERR_INVALID_INPUT:
            puts(RED "Ошибка: Некорректный ввод. Попробуйте снова." RESET);
            break;
        case ERR_EOF_REACHED:
            puts("");
            break;
        case ERR_MTX_DIM_MISMATCH:
            puts(RED "Ошибка: Размерности двух матриц должны совпадать." RESET);
            break;
        case ERR_SUCCESS:
            break;
    }
    return error;
}
