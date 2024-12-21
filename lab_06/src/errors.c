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
        case ERR_NOT_FOUND:
            puts(RED "Студент с такой фамилией не найден." RESET);
            break;
        case ERR_OPENING_FILE:
            puts(RED "Файл с таким именем не может быть открыт или его не существует." RESET);
            break;
        case ERR_NOT_A_NUMBER:
            puts(RED "Некорректный формат введенных данных. Встретились нечисленные данные там, где должно быть число." RESET);
            break;
        case ERR_ALLOCATING_MEMORY:
            puts(RED "Ошибка выделения памяти." RESET);
            break;
        case ERR_DATA:
            puts(RED "Некорректные формат введенных данных." RESET);
            break;
        case ERR_EMPTY_TREE:
            puts(RED "Дерево пустое." RESET);
            break;
        case ERR_GRAPHVIZ_CONVERSION:
            puts(RED "Ошибка преобразования .dot-файла в изображение. Убедитесь, что Graphviz установлен." RESET);
            break;
        case ERR_IS_A_DUBLICATE:
            puts(RED "Информация о студенте с такой фамилией уже хранится в дереве." RESET);
            break;
    }
    return rc;
}
