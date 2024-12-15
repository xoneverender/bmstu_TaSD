#include "menu_actions.h"

err_code_e operate_adding_element(void *stack, err_code_e (*push_func)(void*, data_t*))
{
    err_code_e rc;
    char *buf;
    size_t len = STRING_LEN;
    
    puts("");
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

    printf(BOLD "\nУдаленный элемент: \n" RESET);
    print_data(el);
    puts("\n");

    return ERR_SUCCESS;
}

void operate_printing_array_stack(void *top)
{
    array_stack_t *stack = (array_stack_t *)top;

    if (!stack || stack->top <= 0)
    {
        puts("Стек пуст.");
        return;
    }

    for (int i = stack->top - 1; i >= 0; i--)
    {
        printf("[%d] ", stack->top - i);
        print_data(stack->data[i]); 
        puts("");
    }
}

void operate_printing_list_stack(void *top)
{
    list_stack_t **stack_ptr = (list_stack_t **)top; 

    if (!stack_ptr)
        return;

    list_stack_t *stack = *stack_ptr; 


    if (!stack)
    {
        puts("Стек пуст.");
        return;
    }

    while (stack)
    {
        print_data(stack->data);
        printf(" [ Адрес: %p ]\n", (void *)(stack->data));
        stack = stack->next;
    }
}

void operate_printing_removed(removed_t *removed)
{
    if (!removed || !removed->data)
    {
        puts("Не было удалено ни одного элемента.");
        return;
    }

    for (size_t i = 0; i < removed->count; i++)
        printf("[%zu] Адрес: %p\n", i, removed->data[i]);
}

err_code_e operate_calculating_rpn(void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode, bool verbose)
{
    err_code_e rc;
    data_t *result = NULL;
    void *stack_b = NULL;
    
    if (mode == ARR_STACK)
    {
        stack_b = malloc(sizeof(array_stack_t));
        ((array_stack_t *)stack_b)->top = 0;
    }

    if ((rc = rpn_by_mode(stack, stack_b, pop_func, push_func, free_func, mode)))
        return rc;

    if (mode == ARR_STACK)
        free(stack_b);

    // проверка результата в исходном стеке
    if ((rc = pop_func(stack, &result)) != ERR_SUCCESS)
        return ERR_CALCULATIONS;
    

    if (verbose)
    {
        printf("Результат: ");
        print_data(result);
        puts("");
    }

    free(result);


    return ERR_SUCCESS;
}


err_code_e operate_calculating_rpn_from_string(err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode)
{
    err_code_e rc;
    char *buf;
    size_t len = STRING_LEN;
    void *stack = NULL;
    bool verbose;

    if (mode == ARR_STACK)
    {
        stack = malloc(sizeof(array_stack_t));
        ((array_stack_t *)stack)->top = 0;
    }

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
    
    if (mode == ARR_STACK)
    {
        if ((rc = calculate_rpn_from_string(buf, stack, pop_func, push_func, free_func, mode, verbose = true)))
            return rc;
    }
    else
    {
        if ((rc = calculate_rpn_from_string(buf, &stack, pop_func, push_func, free_func, mode, verbose = true)))
            return rc;
    }

    if (mode == ARR_STACK)
        free(stack);

    return ERR_SUCCESS;
}

err_code_e operate_comparizon(void)
{
    int array_stack_memory = 0, list_stack_memory = 0;
    size_t iterations_count = 100;
    size_t lengths[] = { 10, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    size_t current_len;
    err_code_e rc;
    action_funcs_t array_funcs, list_funcs;
    double size_difference, time_difference;
    long array_calcs_time = 0, list_calcs_time = 0;
    char *expression = NULL;

    puts("------------------------------------------------------------------------------------------------------------------------------------------");
    printf("|%-15s|%-79s|%-68s|\n", "Длина", "Скорость обработки", "Размер");
    printf("|%s |------------------------------------------------------------------------------------------------------------------------------\n", "выражения");
    printf("|%-10s|%-33s|%-35s|%-26s|%-28s|%-19s|%-26s|\n", "", "Стек на списке, тики", "Стек на массиве, тики", "Разница, %", "Стек на списке, байт", "Стек на массиве, байт", "Разница, %");
    puts("------------------------------------------------------------------------------------------------------------------------------------------");

    for (size_t i = 0; i < sizeof(lengths) / sizeof(lengths[0]); i++)
    {   
        current_len = lengths[i];
        rc = get_math_expression(current_len, &expression);
        if (rc) 
            return rc;

        array_calcs_time = 0;
        list_calcs_time = 0;

        for (size_t j = 0; j < iterations_count; j++)
        {
            // Инициализация массивного стека
            array_stack_t *stack_arr = malloc(sizeof(array_stack_t));
            if (!stack_arr) 
                return ERR_ALLOCATING_MEMORY;
            stack_arr->top = 0;

            // Инициализация спискового стека
            list_stack_t *stack_list = NULL;

            // Настройка функций для массивного и спискового стеков
            init_array_stack_funcs(&array_funcs, stack_arr);
            init_list_stack_funcs(&list_funcs, &stack_list);
            
            // Вычисления
            array_calcs_time += count_calculations_time(expression, &array_funcs, ARR_STACK);
            list_calcs_time += count_calculations_time(expression, &list_funcs, LINKED_LIST_STACK);

            free_array(array_funcs.stack);
            free_list(list_funcs.stack);
        }

        // Подсчёт памяти
        array_stack_memory = sizeof(array_stack_t) + STACK_CAPACITY * sizeof(data_t);
        list_stack_memory = lengths[i] * (sizeof(list_stack_t) + sizeof(data_t));

        free(expression);

        // Усреднение времени
        array_calcs_time /= iterations_count;
        list_calcs_time /= iterations_count;

        // Вычисления разницы
        time_difference = (array_calcs_time) ?  100 * ((list_calcs_time - array_calcs_time) / (double)list_calcs_time) : 0;
        size_difference = (array_stack_memory) ? 100 * ((list_stack_memory - array_stack_memory) / (double)list_stack_memory) : 0;

        printf("|%-10zu|%-20lu|%-21lu|%-19.2f|%-20d|%-21d|%-19.2f|\n", current_len, list_calcs_time, array_calcs_time, time_difference, list_stack_memory, array_stack_memory, size_difference);
        puts("------------------------------------------------------------------------------------------------------------------------------------------");
    }

    puts("");
    return ERR_SUCCESS;
}

