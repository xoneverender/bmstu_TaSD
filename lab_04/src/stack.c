#include <stack.h>

err_code_e define_data_type(data_t *data, char *token)
{
    float num;
 char *operators = "+-*/";

    if (is_float(&num, token))
    {
        data->type = OPERAND;
        data->content.value = num;
    }
    else if (strchr(operators, *token))
    {
        data->type = OPERATOR;
        data->content.operator = *token;
    }
    else
        return ERR_INVALID_STACK_INPUT;

    return ERR_SUCCESS;
}

err_code_e push_into_list(void *stack, data_t *data)
{
    node_t **top = (node_t **)stack;

    node_t *new_top = malloc(sizeof(node_t));
    if (!new_top)
        return ERR_ALLOCATING_MEMORY;

    new_top->data = data;
    new_top->next = *top;

    *top = new_top;

    return ERR_SUCCESS;
}

err_code_e push_into_arr(void *stack, data_t *data)
{
    array_stack_t *top = (array_stack_t *)stack;

    if (top->top - 1 >= STACK_CAPACITY)
        return ERR_STACK_OVERFLOW;

    top->data[top->top++] = data;

    return ERR_SUCCESS;
}

err_code_e push(void *stack, char *token, err_code_e (*push_func)(void *, data_t *))
{   
    err_code_e rc;

    data_t *data = malloc(sizeof(data_t));
    if (data == NULL)
        return ERR_ALLOCATING_MEMORY;

    if ((rc = define_data_type(data, token)) != ERR_SUCCESS)
    {
        free(data);
        return ERR_INVALID_STACK_INPUT;
    }

    if ((rc = push_func(stack, data)) != ERR_SUCCESS)
    {
        free(data);
        return rc;
    }

    return ERR_SUCCESS;
}

void free_list(void *stack)
{
    if (!stack)
        return;

    node_t **top = (node_t **)stack;
    node_t *current;
    
    while (*top)
    {
        current = *top;
        *top = (*top)->next;

        free(current->data);
        free(current);
    }
}

void free_array(void *stack)
{
    if (!stack)
        return;

    array_stack_t *top = (array_stack_t *)stack;

    while (top->top--)
        free(top->data[top->top]);
}

void free_stack(void *stack, void (*free_func)(void *))
{
    free_func(stack);
}

err_code_e pop_from_array(void *stack, data_t **el) 
{
    array_stack_t *top = (array_stack_t *)stack;

    if (top == NULL)
        return ERR_NULL_PTR;
    if (top->top <= 0)  // Проверка для пустого стека
        return ERR_EMPTY_STACK;

    *el = top->data[--top->top];  // Возвращаем указатель на удаляемый элемент

    return ERR_SUCCESS;
}

err_code_e pop_from_list(void *stack, data_t **el) 
{
    node_t **top = (node_t **)stack;
    node_t *current;

    if (top == NULL)
        return ERR_NULL_PTR;
    if (*top == NULL)
        return ERR_EMPTY_STACK;

    current = *top;
    *el = current->data;  // Сохраняем данные для возврата
    *top = current->next;

    free(current);  // Освобождаем текущий узел, но не данные

    return ERR_SUCCESS;
}


err_code_e pop(void *stack, removed_t *removed, data_t **el, err_code_e (*pop_func)(void *, data_t **)) 
{
    err_code_e rc;
    void **realoced_data = NULL;

    if ((rc = pop_func(stack, el)) != ERR_SUCCESS)
        return rc;

    // Выделение памяти для массива удалённых элементов
    if (removed->data == NULL) 
    {
        removed->capacity = 50;
        removed->data = malloc(sizeof(void *) * removed->capacity);
        if (!removed->data)
            return ERR_ALLOCATING_MEMORY;
    }

    // Перевыделение памяти при переполнении
    if (removed->capacity <= removed->count) 
    {
        removed->capacity += 50;
        realoced_data = realloc(removed->data, sizeof(void *) * removed->capacity);
        if (!realoced_data)
            return ERR_ALLOCATING_MEMORY;
        removed->data = realoced_data;
    }

    // Добавление адреса удалённого элемента
    removed->data[removed->count++] = *el;

    return ERR_SUCCESS;
}

err_code_e reverse_stack(void *src, void *dst, err_code_e (*push)(void *, data_t *), err_code_e (*pop)(void *, data_t **))
{
    data_t *el;    
    err_code_e rc;

    while (src)
    {
        if ((rc = pop(src, &el)) != ERR_SUCCESS)
            return rc;
        if ((rc = push(dst, el)) != ERR_SUCCESS)
            return rc;
    }

    return ERR_SUCCESS;
}

void print_data(data_t *data)
{
    if (!data)
    {
        puts("NULL.");
        return;
    }

    if (data->type == OPERAND)
    {
        printf("Операнд: %.6f\n", data->content.value);
    }
    else if (data->type == OPERATOR)
    {
        printf("Оператор: %c\n", data->content.operator);
    }
}

void init_action_funcs(action_funcs_t *funcs) 
{
    funcs->free = NULL;
    funcs->pop = NULL;
    funcs->print = NULL;
    funcs->push = NULL;
    funcs->stack = NULL;
}

void init_list_stack_funcs(action_funcs_t *funcs, node_t **stack) 
{
    if (funcs && stack) 
    {
        funcs->push = push_into_list;
        funcs->pop = pop_from_list;
        funcs->free = free_list;
        funcs->print = operate_printing_list_stack;
        funcs->stack = stack;
    }
}

void init_array_stack_funcs(action_funcs_t *funcs, array_stack_t *stack) 
{
    if (funcs && stack) 
    {
        funcs->push = push_into_arr;
        funcs->pop = pop_from_array;
        funcs->free = free_array;
        funcs->print = operate_printing_array_stack;
        funcs->stack = stack;
    }
}

