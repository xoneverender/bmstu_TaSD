#include "menu_actions.h"

err_code_e operate_adding_element(void *stack, err_code_e (*push_func)(void*, data_t*))
{
    err_code_e rc;
    char *buf;
    size_t len = STRING_LEN;
    
    do 
    {
        printf("Введите элемент, который вы хотите добавить в стек. Это может быть вещественное число или любой из символов +-*/.\n");
        input_arrow();

        buf = NULL;
        
        rc = read_string(stdin, &buf, &len);
        puts("");
    } while (rc != ERR_SUCCESS && rc != ERR_EOF_REACHED);

    if (rc == ERR_EOF_REACHED)
        return rc;
    
    if ((rc = push(stack, buf, push_func)) != ERR_SUCCESS)
        return rc;

    return ERR_SUCCESS;
}

err_code_e operate_removing_element(void *stack, removed_t *removed, err_code_e (*pop_func)(void *, data_t **el))
{
    err_code_e rc;
    data_t *el;

    if ((rc = pop(stack, removed, &el, pop_func)) != ERR_SUCCESS)
        return rc;

    printf("Удаленный элемент: ");
    print_data(el);

    return ERR_SUCCESS;
}

void operate_printing_array_stack(void *top)
{
    array_stack_t *stack = (array_stack_t *)top;

    if (!stack || stack->top <= 0)
    {
        puts("Массив пуст.\n");
        return;
    }

    for (int i = stack->top - 1; i >= 0; i--)
    {
        printf("[%d] ", stack->top - i);
        print_data(stack->data[i]); 
    }
    puts("");
}

void operate_printing_list_stack(void *top)
{
    node_t *stack = (node_t *)top;

    node_t *current = stack;

    if (!current)
    {
        puts("Список пуст.\n");
        return;
    }

    while (current)
    {
        print_data(current->data);
        current = current->next;
    }
    puts("");
}

void operate_printing_removed(removed_t *removed)
{
    if (!removed || !removed->data)
    {
        puts("Не было удалено ни одного элемента.");
        return;
    }

    for (size_t i = 0; i < removed->count; i++)
        printf("[%zu] Адресс: %p\n", i, removed->data[i]);
}

err_code_e operate_calculating_rpn(void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *))
{
    err_code_e rc;
    data_t *result = NULL;

    if ((rc = rpn(stack, pop_func, push_func, free_func)))
        return rc;

    // проверка результата в исходном стеке
    if ((rc = pop_func(stack, &result)) != ERR_SUCCESS)
        return ERR_CALCULATIONS;

    printf("Результат: ");
    print_data(result);

    free(result);

    return ERR_SUCCESS;
}

