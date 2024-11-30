#include "calculations.h"

err_code_e rpn(void *stack_a, err_code_e (*pop_func)(void *, data_t **), err_code_e (*push_func)(void *, data_t *), void (*free_func)(void *))
{
    err_code_e rc;
    void *stack_b = NULL;

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

    while (stack_a)
    {
        // снимаем элемент с верхушки стека
        if ((rc = pop_func(stack_a, &current)))
            return ERR_CALCULATIONS;
        
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
