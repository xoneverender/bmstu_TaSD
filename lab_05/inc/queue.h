#ifndef QUEUE
#define QUEUE

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

#define QUEUE_CAPACITY 10000

typedef struct 
{
    double time_since_last; // Интервал между добавлением двух заявок в очередь
    double processing_time; // Время обработки заявки ОА
    double generation_moment; // Время генерации заявки
    double enqueue_time; // Время входа в очередь
} request_t;

typedef struct
{
    int head_ind;
    int tail_ind;
    int elems_count;
    request_t *requests[QUEUE_CAPACITY];
} queue_arr_t;

typedef struct queue_list_t
{
    request_t *request;
    struct queue_list_t *next;
} queue_list_t;

typedef struct 
{
    err_code_e (*enqueue)(void*, request_t *);
    err_code_e (*dequeue)(void*, request_t **);
    err_code_e (*free)(void *);
    err_code_e (*create)(void *);
    void *queue_1;
    void *queue_2;
} action_funcs_t;

err_code_e create_queue_arr(void *queue);
err_code_e create_queue_list(void *queue);
err_code_e enqueue_arr(void *queue, request_t *request);
err_code_e enqueue_list(void *queue, request_t *request);
err_code_e dequeue_arr(void *queue, request_t **request);
err_code_e dequeue_list(void *queue, request_t **request);
err_code_e free_arr(void *queue);
err_code_e free_list(void *queue);
err_code_e init_arr_funcs(action_funcs_t *funcs);
err_code_e init_list_funcs(action_funcs_t *funcs);

#endif /* QUEUE */
