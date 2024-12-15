#include "compare.h"

unsigned long long microseconds_now(void)
{
    struct timeval value;
    gettimeofday(&value, NULL);
    return (unsigned long long)value.tv_sec * 1000ULL * 1000ULL + value.tv_usec;
}

size_t calc_size_list(queue_list_t *q)
{
    size_t sz = 0;
    while (q)
    {
        sz += sizeof(*q);
        q = q->next;
    }
    return sz;
}

err_code_e compare(void)
{
    queue_arr_t *queue_arr;
    queue_list_t *queue_list;

    request_t *request_in = calloc(1, sizeof(request_t));
    request_t *request_out;

    double list_enqueue_time = 0, list_dequeue_time = 0;
    double arr_enqueue_time = 0, arr_dequeue_time = 0;
    unsigned long long beg, end;

    create_queue_arr(&queue_arr);
    create_queue_list(&queue_list);

    // Замер времени добавления элемента в очередь на списке
    for (size_t i = 0; i < MEASURES_COUNT; i++)
    {
        beg = microseconds_now();
        enqueue_list(&queue_list, request_in);
        end = microseconds_now();

        list_enqueue_time += (end - beg);
    }
    list_enqueue_time /= (double)MEASURES_COUNT;

    // Замер времени добавления элемента в очередь на массиве
    for (size_t i = 0; i < MEASURES_COUNT; i++)
    {
        beg = microseconds_now();
        enqueue_arr(queue_arr, request_in);
        end = microseconds_now();

        arr_enqueue_time += (end - beg);
    }
    arr_enqueue_time /= (double)MEASURES_COUNT;

    // Замер времени удаления элемента из очереди на списке
    for (size_t i = 0; i < MEASURES_COUNT; i++)
    {
        beg = microseconds_now();
        dequeue_list(&queue_list, &request_out);
        end = microseconds_now();

        list_dequeue_time += (end - beg);
    }
    list_dequeue_time /= MEASURES_COUNT;

    // Замер времени удаления элемента из очереди на массиве
    for (size_t i = 0; i < MEASURES_COUNT; i++)
    {
        beg = microseconds_now();
        dequeue_arr(queue_arr, &request_out);
        end = microseconds_now();

        arr_dequeue_time += (end - beg);
    }
    arr_dequeue_time /= (double)MEASURES_COUNT;

    puts(BOLD "\nСреднее время добавления элемента:" RESET);
    printf("\tОчередь на массиве: %lf мкс.\n", arr_enqueue_time);
    printf("\tОчередь на списке: %lf мкс.\n", list_enqueue_time);

    puts(BOLD "\nСреднее время удаления элемента:" RESET);
    printf("\tОчередь на массиве: %lf мкс.\n", arr_dequeue_time);
    printf("\tОчередь на списке: %lf мкс.\n\n", list_dequeue_time);

    free(request_in);
    free_arr(queue_arr);
    free_list(&queue_list);

    create_queue_arr(&queue_arr);
    create_queue_list(&queue_list);

    printf("-----------------------------------------------------------------------------------------------------\n");
    printf("| Кол-во элементов | Размер очереди на списке, байт | Размер очереди на массиве, байт | Соотношение |\n");
    printf("|------------------|--------------------------------|---------------------------------|-------------|\n");

    // Последовательно добавляем в очередь STEP элементов, а затем выводим объем занятой памяти
    for (size_t size = 100; size <= QUEUE_CAPACITY; size += STEP)
    {
        for (size_t i = 0; i < STEP; i++)
        {
            request_in = calloc(1, sizeof(request_t));
            enqueue_arr(queue_arr, request_in);
            request_in = calloc(1, sizeof(request_t));
            enqueue_list(&queue_list, request_in);
        }

        printf("%-9s%-10zu%-15s%-18zu%-15s%-19zu%-6s%-8.3lf%s\n", "|", size, "|", calc_size_list(queue_list), "|", sizeof(*queue_arr), "|", (float)sizeof(*queue_arr) / calc_size_list(queue_list), "|");
    }
    printf("-----------------------------------------------------------------------------------------------------\n");


    free_arr(queue_arr);
    free_list(&queue_list);

    return ERR_SUCCESS;
}