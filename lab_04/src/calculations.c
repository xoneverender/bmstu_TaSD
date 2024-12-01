#include "calculations.h"

err_code_e rpn(void *stack_a, void *stack_b, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *))
{
    err_code_e rc;

    if ((rc = reverse_stack(stack_a, stack_b, push_func, pop_func)) != ERR_SUCCESS)
    {
        free_stack(stack_b, free_func);
        return rc;
    }

    // обработка с приоритетом "*" и "/"
    if ((rc = calculate(stack_b, stack_a, "+-", pop_func, push_func)) != ERR_SUCCESS)
    {
        free_stack(stack_b, free_func);
        return rc;
    }

    if ((rc = reverse_stack(stack_a, stack_b, push_func, pop_func)) != ERR_SUCCESS)
    {
        free_stack(stack_b, free_func);
        return rc;
    }

    if ((rc = calculate(stack_b, stack_a, "", pop_func, push_func)) != ERR_SUCCESS)
    {
        free_stack(stack_b, free_func);
        return rc;
    }

    return ERR_SUCCESS;
}


err_code_e calculate(void *stack_a, void *stack_b, char *skipped_signs, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *))
{
    data_t *current = NULL, *current_1 = NULL, *current_2 = NULL, *new = NULL;
    err_code_e rc;

    while (1)
    {
        // снимаем элемент с верхушки стека
        if ((rc = pop_func(stack_a, &current)))
            break;
        
        if (current->type == OPERAND)
        {
            // если полученный элемент - операнд, записываем его во второй стек
            if ((rc = push_func(stack_b, current)))
                return ERR_CALCULATIONS;
        }
        else if (current->type == OPERATOR)
        {
            if (strchr(skipped_signs, current->content.operator))
            {
                // неприоритетные операторы записываем во второй стек
                if ((rc = push_func(stack_b, current)) != ERR_SUCCESS)
                    return ERR_CALCULATIONS;
            }
            else
            {
                if ((rc = pop_func(stack_a, &current_1)) != ERR_SUCCESS)
                {
                    free(current);
                    return ERR_CALCULATIONS;
                }
                if ((rc = pop_func(stack_b, &current_2)) != ERR_SUCCESS)
                {
                    free(current);
                    free(current_1);
                    return ERR_CALCULATIONS;
                }
                
                new = malloc(sizeof(data_t));
                if (new == NULL) 
                {
                    free(current);
                    free(current_1);
                    free(current_2);
                    return ERR_ALLOCATING_MEMORY;
                }

                new->type = OPERAND;

                // вычисляем значение выражения
                switch (current->content.operator)
                {   
                    case '-':
                        new->content.value = current_1->content.value - current_2->content.value;
                        break;
                    case '+':
                        new->content.value = current_1->content.value + current_2->content.value;
                        break;
                    case '*':
                        new->content.value = current_1->content.value * current_2->content.value;
                        break;
                    case '/':
                        if (fabs(current_2->content.value) < 1e-6)
                        {
                            free(current);
                            free(current_1);
                            free(current_2);
                            free(new);
                            return ERR_DIVISION_BY_ZERO;
                        }
                        new->content.value = current_1->content.value / current_2->content.value;
                        break;
                }

                // сохраняем результат на стек
                if ((rc = push_func(stack_b, new)))
                {
                    free(new);
                    return ERR_CALCULATIONS;
                }

                free(current);
                free(current_1);
                free(current_2);
            }
        }

    }
    return ERR_SUCCESS;
}

unsigned long long count_calculations_time(char *string, action_funcs_t *funcs, mode_e mode)
{
    struct timeval start, end;
    unsigned long long start_us, end_us;

    gettimeofday(&start, NULL);
    (void)calculate_rpn_from_string(string, funcs->stack, funcs->pop, funcs->push, funcs->free, mode);
    gettimeofday(&end, NULL);

    start_us = (unsigned long long)start.tv_sec * 1000000ULL + start.tv_usec;
    end_us = (unsigned long long)end.tv_sec * 1000000ULL + end.tv_usec;

    return end_us - start_us;
}

err_code_e calculate_rpn_from_string(char *buf, void *stack, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *), mode_e mode)
{
    err_code_e rc;

    if ((rc = parse_string(buf, stack, push_func)) != ERR_SUCCESS)
        return rc;

    if ((rc = operate_calculating_rpn(stack, pop_func, push_func, free_func, mode)))
        return rc;

    return ERR_SUCCESS;
}
