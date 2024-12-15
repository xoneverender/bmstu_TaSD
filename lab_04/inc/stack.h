#ifndef STACK
#define STACK

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"

#define STACK_CAPACITY 1000

typedef enum
{
    OPERAND, 
    OPERATOR 
} data_type_e;

typedef struct
{
    data_type_e type; 
    union
    {
        char operator;    
        float value;
    } content;  
} data_t;   

typedef struct 
{
    data_t *data[STACK_CAPACITY];
    int top;                 
} array_stack_t;

typedef struct list_stack_t
{
    struct list_stack_t *next;
    data_t *data;
} list_stack_t;

typedef struct
{
    void **data;
    size_t count;
    size_t capacity;
} removed_t;

typedef struct 
{
    err_code_e (*push)(void*, data_t *);
    err_code_e (*pop)(void*, data_t**);
    void (*free)(void*);
    void (*print)(void *);
    void *stack;
} 
action_funcs_t;

#include "menu_actions.h"

#include "str.h"

err_code_e define_data_type(data_t *data, char *token);
err_code_e push_into_list(void *stack, data_t *data);
err_code_e push_into_arr(void *stack, data_t *data);
err_code_e push(void *stack, char *token, err_code_e (*push_func)(void *, data_t *));
void free_list(void *stack);
void free_array(void *stack);
void free_stack(void *stack, void (*free_func)(void *));
err_code_e pop_from_array(void *stack, data_t **el);
err_code_e pop_from_list(void *stack, data_t **el);
err_code_e pop(void *stack, removed_t *removed, data_t **el, err_code_e (*pop_from)(void *, data_t **el));
err_code_e reverse_stack(void *src, void *dst, err_code_e (*push)(void *, data_t *), err_code_e (*pop)(void *, data_t **));
void print_data(data_t *data);
void init_action_funcs(action_funcs_t *funcs);
void init_array_stack_funcs(action_funcs_t *funcs, array_stack_t *stack);
void init_list_stack_funcs(action_funcs_t *funcs, list_stack_t **stack);

#endif /* STACK */
