#include "simulation.h"

err_code_e simulate(void)
{
    err_code_e rc;
    visibility_e adress_visibility;
    time_settings_e time_settings;
    mode_e mode;
    timing_data_t timings;
    action_funcs_t funcs;
    size_t counter = 0, processed_1 = 0, processed_2 = 0, entered_1 = 0, entered_2 = 0;
    request_handler_t generator_1, generator_2, processor;
    request_type_e processed_type;
    queue_event_t events;
    double current_time = 0.0, idle_time = 0.0;
    double avg_queue_length_1 = 0.0, avg_queue_length_2 = 0.0;
    double queue_length_sum_1 = 0.0, queue_length_sum_2 = 0.0;
    double avg_queue_time_1 = 0.0, avg_queue_time_2 = 0.0;
    double queue_time_sum_1 = 0.0, queue_time_sum_2 = 0.0;
    double prev_time = 0.0;
    void *queue_1 = NULL, *queue_2 = NULL;

    // Сбор данных для запуска симуляции: пользовательские настройки интервалов, видимость адресов заявок, вид очереди (список/массив)
    if ((rc = collect_data_for_simulation(&adress_visibility, &time_settings, &mode)))
        return rc;

    // Настрока временных интервалов (дефолт/предпочтения пользователя)
    if ((rc = get_time_intervals(&timings, time_settings)))
        return rc;

    if (mode == LIST)
    {
        funcs.queue_1 = &queue_1;
        funcs.queue_2 = &queue_2;
    }
    else
    {
        funcs.queue_1 = queue_1;
        funcs.queue_2 = queue_2;
    }

    // Инициализация функций для работы с заданным видом очереди
    if (mode == LIST)
    {
        if ((rc = init_list_funcs(&funcs)))
            return rc;
    }
    else if (mode == ARRAY)
    {
        if ((rc = init_arr_funcs(&funcs)))
            return rc;
    }

    // Инициализация очередей
    if (mode == ARRAY)
    {
        if ((rc = funcs.create(&funcs.queue_1)))
            return rc;
        if ((rc = funcs.create(&funcs.queue_2)))
            return rc;
    }

    generator_1.request = NULL;
    generator_2.request = NULL;
    processor.request = NULL;

    events.next_add_type1 = -1;
    events.next_add_type2 = -1;
    events.next_remove = -1;

    processed_type = FIRST;

    // Симуляция
    while (counter < MAX_REQUESTS / CHECK_POINT)
    {
        while (processed_1 < (counter + 1) * CHECK_POINT)
        {
            // Если подошло время добавить первую заявку в очередь
            if (generator_1.request && events.next_add_type1 == current_time)
            {
                generator_1.request->enqueue_time = current_time;

                if ((rc = funcs.enqueue(funcs.queue_1, generator_1.request)))
                {
                    free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                    return rc;
                }
                entered_1++;       
                generator_1.request = NULL;
            }
            // Если первый генератор пуст, генерируем заявку
            if (generator_1.request == NULL)
            {
                if ((rc = generate_request(&generator_1.request, timings, FIRST, current_time)))
                {
                    free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                    return rc;
                }
                events.next_add_type1 = current_time + generator_1.request->time_since_last;
            }

            // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
            // puts("Строка 82. Массив №1.");
            // print_queue(funcs.queue_1, mode);
            // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG

            
            // Если подошло время добавить вторую заявку в очередь
            if (generator_2.request && events.next_add_type2 == current_time)
            {
                generator_2.request->enqueue_time = current_time;

                if ((rc = funcs.enqueue(funcs.queue_2, generator_2.request)))
                {
                    // puts("Строка 95. Тут.");
                    // free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                    return rc;
                }   
                entered_2++;             
                generator_2.request = NULL;
            }
            // Если второй генератор пуст, генерируем заявку
            if (generator_2.request == NULL)
            {
                if ((rc = generate_request(&generator_2.request, timings, SECOND, current_time)))
                {
                    free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                    return rc;
                }
                events.next_add_type2 = current_time + generator_2.request->time_since_last;
            }

            // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
            // puts("Строка 109. Массив №2.");
            // print_queue(funcs.queue_2, mode);
            // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG

            // Если подошло время снять заявку с ОА
            if (processor.request && events.next_remove == current_time)
            {
                free(processor.request);
                processor.request = NULL;

                if (processed_type == FIRST)
                    processed_1++;
                else if (processed_type == SECOND)
                    processed_2++;
            }
            // Проверка того, можно ли на текущем этапе поместить заявку в очередь
            if (processor.request == NULL)
            {
                // Если первая очередь не пустая, помещаем заявку из очереди в ОА
                if (!is_empty(funcs.queue_1, mode))
                {
                    
                    // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
                    // puts("Строка 143. Массив №1 не пустой.");
                    // puts("Строка 129. Массив №1.");
                    // print_queue(&funcs.queue_1, mode);
                    // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG

                    if ((rc = funcs.dequeue(funcs.queue_1, &processor.request)))
                    {
                        free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                        return rc;
                    }     

                    queue_time_sum_1 += current_time - processor.request->enqueue_time;
                    processed_type = FIRST;
                    events.next_remove = current_time + processor.request->processing_time;
                }
                // Если первая пустая, а вторая нет, помещаем заявку из второй в ОА
                else if (!is_empty(funcs.queue_2, mode))
                {

                    // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG
                    // puts("Строка 143. Массив №2 не пустой.");
                    // puts("Строка 144. Массив №2.");
                    // print_queue(funcs.queue_2, mode);
                    // DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG DEBUG

                    if ((rc = funcs.dequeue(funcs.queue_2, &processor.request)))
                    {
                        free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);
                        return rc;
                    }
                    queue_time_sum_2 += current_time - processor.request->enqueue_time;
                    processed_type = SECOND;
                    events.next_remove = current_time + processor.request->processing_time;
                }
                // Если обе очереди пустые и процессор простаивает, записываем время простоя
                else
                {
                    if (events.next_add_type1 < events.next_add_type2)
                        idle_time += events.next_add_type1 - current_time;
                    else
                        idle_time += events.next_add_type2 - current_time;
                    events.next_remove = -1;
                }
            }
            // Сбор данных для подсчета средней длины
            queue_length_sum_1 += queue_length(funcs.queue_1, mode) * (current_time - prev_time);
            queue_length_sum_2 += queue_length(funcs.queue_2, mode) * (current_time - prev_time);

            prev_time = current_time;

            // Обновление текущего времени (поиск ближайшего времени, когда произойдет какое-то событие)
            current_time = get_min_positive_time(events);
        }
        counter++;
        
        // Подсчет средней длины очередей
        avg_queue_length_1 = queue_length_sum_1 / current_time;
        avg_queue_length_2 = queue_length_sum_2 / current_time;

        // Подсчет среднего времени пребывания заявки в очереди (учитываем только те, что к этому моменту вышли из очереди)
        avg_queue_time_1 = queue_time_sum_1 / processed_1;
        avg_queue_time_2 = queue_time_sum_2 / processed_2;

        results_for_one_queue(queue_length(funcs.queue_1, mode), avg_queue_length_1, avg_queue_time_1, entered_1, processed_1, FIRST);
        results_for_one_queue(queue_length(funcs.queue_2, mode), avg_queue_length_2, avg_queue_time_2, entered_2, processed_2, SECOND);
    }

    summary(prev_time, idle_time);

    free_all(&generator_1, &generator_2, &processor, funcs.queue_1, funcs.queue_2, funcs.free);

    return ERR_SUCCESS;
}


void print_queue(void *queue, mode_e mode) 
{
    if (!queue) 
    {
        printf("Очередь пуста или не инициализирована.\n");
        return;
    }

    if (mode == ARRAY) 
    {
        queue_arr_t *arr_ptr = (queue_arr_t *)queue;

        if (arr_ptr->elems_count == 0) 
        {
            printf("Очередь пуста.\n");
            return;
        }
        else
        {
            printf("Элементов в массиве: %d\n", arr_ptr->elems_count);
        }

        printf("Индекс головы: %d\n", arr_ptr->head_ind);
        printf("Очередь (массив):\n");
        for (int i = 0; i < arr_ptr->elems_count; i++) 
        {
            int index = (arr_ptr->head_ind + i) % QUEUE_CAPACITY;
            printf("Элемент %d: время генерации = %.2f, время обработки = %.2f\n",
                   i + 1, arr_ptr->requests[index]->generation_moment, arr_ptr->requests[index]->processing_time);
        }
    } 
    else if (mode == LIST) 
    {
        queue_list_t *list_ptr = (queue_list_t *)queue;

        if (!list_ptr) 
        {
            printf("Очередь пуста.\n");
            return;
        }

        printf("Очередь (список):\n");
        int count = 1;
        while (list_ptr) 
        {
            if (list_ptr->request == NULL) 
            {
                printf("Элемент %d: NULL (ошибка данных)\n", count++);
                list_ptr = list_ptr->next;
                continue;
            }

            printf("Элемент %d: время генерации = %.2f, время обработки = %.2f\n",
                   count++, list_ptr->request->generation_moment, list_ptr->request->processing_time);
            list_ptr = list_ptr->next;
        }
    } 
    else 
    {
        printf("Неизвестный тип очереди.\n");
    }
}

void results_for_one_queue(int curr_len, double avg_len, double avg_time_in_queue, int entered, int exited, request_type_e type)
{
    if (type == FIRST)
        printf("====== Обработано %d заявок первого типа ======\n", exited);

    printf(BOLD "Для очереди %d\n" RESET, (int)type);
    printf("Текущая длина очереди: %d\n", curr_len);
    printf("Средняя длина очереди: %lf\n", avg_len);
    printf("Количество вошедших заявок: %d\n", entered);
    printf("Количество вышедших заявок: %d\n", exited);
    printf("Среднее время пребывания в очереди: %lf\n\n", avg_time_in_queue);
} 

void summary(double simulation_time, double idle_time)
{   
    printf(BOLD "Время простоя обслуживающего аппарата: %lf\n" RESET, idle_time);
    printf(BOLD "Время моделирования составило %lf\n" RESET, simulation_time);
}

int queue_length(void *queue, mode_e mode)
{
    queue_arr_t *arr_ptr;
    queue_list_t *list_ptr;
    int length = 0;

    if (!queue)
        return -1;

    if (mode == ARRAY)
    {
        arr_ptr = (queue_arr_t *)queue;
        length = arr_ptr->elems_count;
    }
    else if (mode == LIST)
    {
        list_ptr = *(queue_list_t **)queue;

        while (list_ptr)
        {
            list_ptr = list_ptr->next;
            length += 1;
        }
    }

    return length;
}

void free_all(request_handler_t *generator_1, request_handler_t *generator_2, request_handler_t *processor, void *queue_1, void *queue_2, err_code_e (*free_func)(void *))
{
    if (processor->request != NULL)
        free(processor->request);
    if (generator_1->request != NULL)
        free(generator_1->request);
    if (generator_2->request != NULL)
        free(generator_2->request);

    (void)free_func(queue_1);
    (void)free_func(queue_2);
}

bool is_empty(void *queue, mode_e mode)
{
    queue_arr_t *arr_ptr;
    queue_list_t *list_ptr;

    if (mode == ARRAY)
    {
        arr_ptr = (queue_arr_t *)queue;
        return (arr_ptr->elems_count == 0);
    }
    else
    {
        list_ptr = *(queue_list_t **)queue;
        return (list_ptr == NULL);
    }
}

double get_min_positive_time(queue_event_t events) 
{
    double min_time = FLT_MAX;

    if (events.next_add_type1 < min_time)
        min_time = events.next_add_type1;

    if (events.next_add_type2 < min_time)
        min_time = events.next_add_type2;

    // Время снятия с ОА заявки считается отрицательным, если ОА простаивает => тут обрабатывается только положительное
    if (events.next_remove > 0 && events.next_remove < min_time)
        min_time = events.next_remove;

    return min_time;
}

err_code_e collect_data_for_simulation(visibility_e *adress_visibility, time_settings_e *time_settings, mode_e *mode)
{
    err_code_e rc;
    char *prompt, *error_message;
    long choice;

    prompt = "Выберите способ реализации очереди:\n1. Массив\n2. Список";
    error_message = "Выбранный пункт меню должен быть целым числом от 1 до 2. Повторите ввод.";
    if ((rc = get_int_from_stdin(prompt, &choice, 1, 2, error_message)) != ERR_SUCCESS)
        return rc;
    *mode = (mode_e)choice;
    puts("");

    if (*mode == LIST)
    {
        prompt = "Нужно ли выводить адреса добавляемых/удаляемых заявок?\n1. Да\n2. Нет";
        error_message = "Выбранный пункт меню должен быть целым числом от 1 до 2. Повторите ввод.";
        if ((rc = get_int_from_stdin(prompt, &choice, 1, 2, error_message)) != ERR_SUCCESS)
            return rc;
        *adress_visibility = (visibility_e)choice;
        puts("");
    }
    else
    {
        *adress_visibility = HIDE;
        puts("");
    }

    prompt = "Хотите изменить интервалы поступления и обработки заявок? \n1. Да\n2. Нет";
    error_message = "Выбранный пункт меню должен быть целым числом от 1 до 2. Повторите ввод.";
    if ((rc = get_int_from_stdin(prompt, &choice, 1, 2, error_message)) != ERR_SUCCESS)
        return rc;
    *time_settings = (time_settings_e)choice;
    puts("");

    return ERR_SUCCESS;
}

err_code_e get_time_intervals(timing_data_t *timings, time_settings_e time_settings)
{
    err_code_e rc;

    if (time_settings == UNCHANGED)
    {
        timings->interval_type1.start = 1;
        timings->interval_type1.end = 5;
        timings->interval_type2.start = 0;
        timings->interval_type2.end = 3;
        timings->processing_time1.start = 0;
        timings->processing_time1.end = 4;
        timings->processing_time2.start = 0;
        timings->processing_time2.end = 1;
    }
    else
    {
        if ((rc = set_interval(&timings->interval_type1)))
            return rc;
        if ((rc = set_interval(&timings->interval_type2)))
            return rc;
        if ((rc = set_interval(&timings->processing_time1)))
            return rc;
        if ((rc = set_interval(&timings->processing_time2)))
            return rc;
    }
    return ERR_SUCCESS;
}

err_code_e set_interval(interval_t *interval)
{
    err_code_e rc;
    char *prompt, *error_message;
    double border_1, border_2;

    prompt = "Введите вещественную границу интервала:\n";
    error_message = "Граница интервала должна быть вещественным числом. Повторите ввод.";
    if ((rc = get_double_from_stdin(prompt, &border_1, error_message)))
        return rc;

    prompt = "Введите вещественную границу интервала:\n";
    error_message = "Граница интервала должна быть вещественным числом. Повторите ввод.";
    if ((rc = get_double_from_stdin(prompt, &border_2, error_message)))
        return rc;

    if (border_1 > border_2)
    {
        interval->start = border_2;
        interval->end = border_1;
    }
    else
    {
        interval->start = border_1;
        interval->end = border_2;
    }

    return ERR_SUCCESS;
}

err_code_e generate_request(request_t **request, timing_data_t timings, request_type_e type, double generation_moment)
{
    interval_t interval, processing_time;

    *request = malloc(sizeof(request_t));
    if (!*request)
        return ERR_ALLOCATING_MEMORY;
    
    if (type == FIRST)
    {
        interval = timings.interval_type1;
        processing_time = timings.processing_time1;
    }
    else
    {
        interval = timings.interval_type2;
        processing_time = timings.processing_time2;
    }

    (*request)->time_since_last = random_double(interval.start, interval.end);
    (*request)->processing_time = random_double(processing_time.start, processing_time.end);
    (*request)->generation_moment = generation_moment;
    (*request)->enqueue_time = -1;

    return ERR_SUCCESS;
}
