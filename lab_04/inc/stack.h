#ifndef STACK
#define STACK

#include <stdio.h>
#include <stdlib.h>
#include "errors.h"
#include "str.h"

#define STACK_LEN 1000

typedef struct 
{
    char data[STACK_LEN];
    int top;                 
} array_stack_t;

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

typedef struct node_t
{
    struct node *next;
    data_t data;       
} node_t;

#endif /* STACK */
