#include "files.h"

err_code_e get_math_expression(int length, char *expression)
{
    FILE *f;
    char command[256];
    size_t bytes_read;

    if (length < 3)
        return ERR_INVALID_EXPRESSION_LEN;

    snprintf(command, sizeof(command), "python3 generate_expression.py %d", length);
    system(command);

    f = open("expression.txt", "r");
    if (f == NULL)
        return ERR_OPENING_FILE;

    expression = malloc(sizeof(char) * (length + 1));
    if (!expression)
    {
        fclose(f);
        return ERR_ALLOCATING_MEMORY;
    }

    bytes_read = fread(expression, sizeof(char), length, f);
    if (bytes_read != length) 
    {
        free(expression);
        fclose(f);
        return ERR_INVALID_DATA_IN_FILE;
    }

    expression[length] = '\0';

    fclose(f);

    return ERR_SUCCESS;
}
