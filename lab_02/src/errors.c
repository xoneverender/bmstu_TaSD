#include "errors.h"

void process_error(err_code error_code)
{
    switch(error_code)
    {
        case ERR_OPENING_FILE:
            puts("Данный файл не может быть открыт или его не существует.");
            break;
        case ERR_TOO_LONG_FILENAME:
            printf("Введено слишком длинное название файла. Убедитесь, что длина названия не превышает %d символов латинского алфавита.\n", MAX_STRING_LEN);
            break;
        case ERR_FILENAME_NOT_PROVIDED:
            puts("Новый файл не был выбран. Будет продолжаться работа со старым файлом.");
            break;
        case ERR_INVALID_RECORDS_NUM:
            puts("Заявленное количество записей в файле не является целым положительным числом от 40 до 1000 либо отличается от реального количества.");
            break;
        case ERR_EMPTY_FILE:
            puts("Файл пустой.");
            break;
        case ERR_INVALID_FILE_FORMAT:
            puts("Формат этого файла не соответствует формату, с которым работает программа.");
            break;
        case ERR_ADDING_RECORD:
            puts("В файле уже 1000 записей. Новая запись не может быть добавлена в файл, поскольку тогда он станет непригоден для работы.");
            puts("Удалите запись, чтобы добавить новую.");
            break;
        case ERR_DELITING_RECORD:
            puts("В файле всего 40 записей. Запись не может быть удалена из файла, поскольку тогда он станет непригоден для работы.");
            break;
        case ERR_NOT_DELETED:
            puts("Запись не была удалена.");
            break;
        case ERR_NOT_FOUND:
            puts("Романы автора не были найдены в списке литературы.");
            break;
        case ERR_STRING_TOO_LONG:
            puts("Введена слишком длинная строка. Попробуйте снова.");
            clear_input_buffer();
            break;
        case ERR_EMPTY_STRING:
            puts("Введена пустая строка. Повторите ввод.");
            break;
        case ERR_SUCCESS:
            break;
    }
}
