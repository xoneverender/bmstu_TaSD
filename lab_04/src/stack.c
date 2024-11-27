#include <stack.h>

err_code_e define_data_type(data_type_e *type, char *data)
{
    float num;
    char *operators = "+-*/";

    type = NULL;

    if (is_float(&num, data))
    {
        type = OPERAND;
        return ERR_SUCCESS;
    }
    else if (strpbrk(data, operators))
    {
        type = OPERATOR;
        return ERR_SUCCESS;
    }

    return ERROR_INVALID_STACK_INPUT;
}

err_code_e push_into_list(node_t **new, node_t *top, char *data)
{
    data_type_e type;
    err_code_e rc = ERR_SUCCESS;
    
    *new = malloc(sizeof(node_t));
    
    if ((rc = define_data_type(data)) != ERR_SUCCESS)
    {
        free(new);
        return rc;
    }

    if (type == OPERAND)
        (**new).data.value = atoi(data);
    else if (type == OPERATOR)
        (**new).data.operator = data;

    (**new).next = top;

    return ERR_SUCCESS;
}

err_code_e push_into_arr(array_stack_t **new, )
