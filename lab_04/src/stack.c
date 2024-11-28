#include <stack.h>

err_code_e define_data_type(data_t *data, char *token)
{
    float num;
    char *operators = "+-*/";

    if (is_float(&num, token))
    {
        data->type = OPERAND;
        data->content.value = num;
        return ERR_SUCCESS;
    }
    else if (strpbrk(token, operators))
    {
        data->type = OPERATOR;
        data->content.operator = token[0];
        return ERR_SUCCESS;
    }

    return ERROR_INVALID_STACK_INPUT;
}

err_code_e push_into_list(node_t **top, char *token)
{
    err_code_e rc = ERR_SUCCESS;
    data_t *data;
    node_t *new_top;

    data = malloc(sizeof(data_t));
    if (data == NULL)
        return ERR_ALLOCATING_MEMORY;

    if ((rc = define_data_type(data, token)) != ERR_SUCCESS)
    {
        free(data);
        return ERROR_INVALID_STACK_INPUT;
    }

    new_top = malloc(sizeof(node_t));
    if (new_top == NULL)
        return ERR_ALLOCATING_MEMORY;

    new_top->data = data;
    new_top->next = *top;

    *top = new_top;

    return ERR_SUCCESS;
}

err_code_e push_into_arr(array_stack_t *top, char *token)
{
    err_code_e rc = ERR_SUCCESS;

    data_t *data = malloc(sizeof(data_t));
    if (data == NULL)
        return ERR_ALLOCATING_MEMORY;

    if ((rc = define_data_type(data, token)) != ERR_SUCCESS)
    {
        free(data);
        return ERROR_INVALID_STACK_INPUT;
    }

    top->data[top->top++] = data;

    return ERR_SUCCESS;
}

void free_list(node_t **top)
{
    node_t *old_top;

    while (*top != NULL)
    {
        old_top = *top;
        free((*top)->data);
        *top = (*top)->next;
        free(old_top);
    }
}

void free_array(array_stack_t *top)
{
    data_t *temp;

    for (size_t i = 0; i < top->data[top->top]; i++)
        free(top->data[i]);
}


// я правильно понимаю что удаляем прям с верхушки стека один элемент и все?
err_code_e remove_from_list(node_t **stack_top, node_t **removed_top)
{
    // 
    node_t *next_top = (*stack_top)->next;

    free((*stack_top)->data);
    free(*stack_top);

    *stack_top = next_top;
}
