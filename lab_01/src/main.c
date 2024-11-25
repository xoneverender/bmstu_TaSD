#include "data_processing.h"
#include "user_interface.h"
#include "math_operations.h"
#include "validations.h"
#include "consts.h"
#include "structs.h"
#include "errors.h"

int main(void)
{
    int rc;
    number_t num1 = { 0 }, num2 = { 0 }, res = { 0 };
    char buffer[MAX_MANTISSA_SIZE2 + MAX_EXP_SIZE + 5];

    show_program_purpose();

    if ((rc = read_line(buffer, sizeof(buffer), MAX_MANTISSA_SIZE1)) != ERR_SUCCESS)
        process_error(rc);

    if ((rc = parse_number(buffer, &num1, MAX_MANTISSA_SIZE1)) != ERR_SUCCESS)
        process_error(rc);

    if ((rc = read_line(buffer, sizeof(buffer), MAX_MANTISSA_SIZE2)) != ERR_SUCCESS)
        process_error(rc);

    if ((rc = parse_number(buffer, &num2, MAX_MANTISSA_SIZE2)) != ERR_SUCCESS)
        process_error(rc);
    
    if (is_null(&num1) || is_null(&num2))
        zero_result(&res);
    else
    {
        product(&num1, &num2, &res);

        if ((rc = normalize_result(&res)) != ERR_SUCCESS)
            process_error(rc);
    }

    print_result(&res);

    return ERR_SUCCESS;
}
