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
    }
}