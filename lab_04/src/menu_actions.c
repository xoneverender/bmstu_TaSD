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

    free(buf);
    buf = NULL;

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
    node_t **stack_ptr = (node_t **)top; 
    node_t *stack = *stack_ptr; 

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

err_code_e operate_calculating_rpn(void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode)
{
    err_code_e rc;
    data_t *result = NULL;
    void *stack_b = NULL;

    if (mode == ARR_STACK)
    {
        stack_b = malloc(sizeof(array_stack_t));
        ((array_stack_t *)stack_b)->top = 0;
    }

    if ((rc = rpn(stack, stack_b, pop_func, push_func, free_func)))
        return rc;

    if (mode == ARR_STACK)
        free(stack_b);

    // проверка результата в исходном стеке
    if ((rc = pop_func(stack, &result)) != ERR_SUCCESS)
        return ERR_CALCULATIONS;

    printf("Результат: ");
    print_data(result);

    free(result);


    return ERR_SUCCESS;
}

err_code_e operate_calculating_rpn_from_string(void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode)
{
    err_code_e rc;
    char *buf;
    size_t len = STRING_LEN;

    do 
    {
        printf("Введите выражение, состоящее из вещественных чисел и любых операторов из набора +-*/.\n");
        input_arrow();

        buf = NULL;
        
        rc = read_string(stdin, &buf, &len);
        puts("");
    } while (rc != ERR_SUCCESS && rc != ERR_EOF_REACHED);

    if (rc == ERR_EOF_REACHED)
        return rc;

    if ((rc = calculate_rpn_from_string(buf, stack, pop_func, push_func, free_func, mode)))
        return rc;

    return ERR_SUCCESS;
}

void operate_comparizon()
{
    size_t iterations_count = 100;
    size_t lengths[7] = { 5, 50, 250, 500, 1000, 5000, 50000};
    size_t current_len;
    err_code_e rc;
    action_funcs_t array_funcs, list_funcs;
    unsigned long long array_calcs_time = 0, list_calcs_time = 0;
    char *expression;

    // Инициализация массивного стека
    array_stack_t *stack_arr = malloc(sizeof(array_stack_t));
    if (!stack_arr) 
        return rc;
    stack_arr->top = 0;

    // Инициализация спискового стека
    node_t *stack_list = NULL;

    // Настройка функций для массивного и спискового стеков
    init_array_stack_funcs(&array_funcs, stack_arr);
    init_list_stack_funcs(&list_funcs, &stack_list);

    for (size_t i = 0; i < 7; i++)
    {   
        for (size_t j = 0; j < iterations_count; j++)
        {
            // Генерация выражения для обработки
            current_len = lengths[i];
            do
            {
                rc = get_math_expression(current_len, expression);
            } while (rc);
            
            // Вычисления в стеке, представленном массивом
            array_calcs_time += count_calculations_time(expression, &array_funcs, ARR_STACK);

            // Вычисления в стеке, представленным списком
            list_calcs_time += count_calculations_time(expression, &list_funcs, LINKED_LIST_STACK);
        }
    }
}
