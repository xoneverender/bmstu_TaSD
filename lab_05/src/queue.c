#include "queue.h"

err_code_e create_queue_arr(void *queue) 
{
    queue_arr_t **arr_ptr = (queue_arr_t **)queue;

    *arr_ptr = malloc(sizeof(queue_arr_t));
    if (!*arr_ptr)
        return ERR_ALLOCATING_MEMORY;

    (*arr_ptr)->elems_count = 0;
    (*arr_ptr)->head_ind = 0;
    (*arr_ptr)->tail_ind = -1;

    return ERR_SUCCESS;
}

err_code_e create_queue_list(void *queue) 
{
    queue_list_t **list_ptr = (queue_list_t **)queue;

    *list_ptr = NULL;

    return ERR_SUCCESS;
}

err_code_e enqueue_arr(void *queue, request_t *request)
{   
    queue_arr_t *arr_ptr;

    if (!queue || !request) 
        return ERR_NULL_PTR;

    arr_ptr = (queue_arr_t *)queue;

    if (arr_ptr->elems_count >= QUEUE_CAPACITY)
        return ERR_QUEUE_OVERFLOW;

    // Определение индекса добавления нового элемента для кольцевой очереди
    arr_ptr->tail_ind = (arr_ptr->tail_ind + 1) % QUEUE_CAPACITY;

    arr_ptr->requests[arr_ptr->tail_ind] = request;

    arr_ptr->elems_count++;

    return ERR_SUCCESS;
}

err_code_e enqueue_list(void *queue, request_t *request) 
{
    queue_list_t **list_ptr;
    queue_list_t *new_node;
    queue_list_t *temp;

    if (!queue || !request)
        return ERR_NULL_PTR;

    list_ptr = (queue_list_t **)queue;

    // Создание  && инициализация нового узла списка
    new_node = malloc(sizeof(queue_list_t));
    if (!new_node)
        return ERR_ALLOCATING_MEMORY;

    new_node->request = request;
    new_node->next = NULL;

    if (*list_ptr == NULL) 
        *list_ptr = new_node;
    else 
    {
        // Поиск последнего элементв в списке
        temp = *list_ptr;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = new_node;
    }

    return ERR_SUCCESS;
}

err_code_e dequeue_arr(void *queue, request_t **request)
{
    queue_arr_t *arr_ptr;

    if (!queue) 
        return ERR_NULL_PTR;

    arr_ptr = (queue_arr_t *)queue;

    if (arr_ptr->elems_count == 0)
        return ERR_EMPTY_QUEUE;

    *request = arr_ptr->requests[arr_ptr->head_ind];
    // free(arr_ptr->requests[arr_ptr->head_ind]);
    arr_ptr->requests[arr_ptr->head_ind] = NULL;

    arr_ptr->head_ind = (arr_ptr->head_ind + 1) % QUEUE_CAPACITY;
    arr_ptr->elems_count--;

    return ERR_SUCCESS;
}

err_code_e dequeue_list(void *queue, request_t **request)
{
    queue_list_t **list_ptr;
    queue_list_t *current_node;

    if (!queue)
        return ERR_NULL_PTR;

    list_ptr = (queue_list_t **)queue;

    if (*list_ptr == NULL)
        return ERR_EMPTY_QUEUE;

    *request = (*list_ptr)->request;

    current_node = *list_ptr;
    // free(current_node->request);
    *list_ptr = current_node->next;
    free(current_node);

    return ERR_SUCCESS;
}

err_code_e free_arr(void *queue)
{
    queue_arr_t *arr_ptr;
    int index;

    if (!queue) 
        return ERR_NULL_PTR;

    arr_ptr = (queue_arr_t *)queue;

    for (int i = 0; i < arr_ptr->elems_count; i++) 
    {
        // Индекс с учётом кольцевой очереди
        index = (arr_ptr->head_ind + i) % QUEUE_CAPACITY; 
        free(arr_ptr->requests[index]);
    }

    free(arr_ptr);

    return ERR_SUCCESS;
}

err_code_e free_list(void *queue)
{
    queue_list_t **list_ptr;
    queue_list_t *temp;

    if (!queue) 
        return ERR_NULL_PTR;

    list_ptr = (queue_list_t **)queue;

    while (*list_ptr) 
    {
        temp = *list_ptr;
        *list_ptr = (*list_ptr)->next;
        free(temp->request);      
        free(temp);       
    }
    return ERR_SUCCESS;
}

err_code_e init_arr_funcs(action_funcs_t *funcs)
{
    if (!funcs) 
        return ERR_NULL_PTR;

    funcs->enqueue = enqueue_arr;
    funcs->dequeue = dequeue_arr;
    funcs->free = free_arr;
    funcs->create = create_queue_arr;

    return ERR_SUCCESS;
}

err_code_e init_list_funcs(action_funcs_t *funcs)
{
    if (!funcs) 
        return ERR_NULL_PTR;

    funcs->enqueue = enqueue_list; 
    funcs->dequeue = dequeue_list; 
    funcs->free = free_list;       
    funcs->create = create_queue_list;
    
    return ERR_SUCCESS;
}
