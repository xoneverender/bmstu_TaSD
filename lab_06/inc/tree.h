#ifndef TREE
#define TREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "errors.h"

typedef struct
{
    char *surname;       // Фамилия студента
    double average_mark; // Средний балл студента
} student_t;

// Структура узла дерева
typedef struct tree_node 
{
    student_t *student;  // Информация о студенте
    struct tree_node *left;  // Левый потомок
    struct tree_node *right; // Правый потомок
} tree_node_t;

int strings_compare(student_t *student_1, student_t *student_2);
int double_compare(student_t *student_1, student_t *student_2);
tree_node_t* create_node(student_t *student);
student_t* create_student(char *surname, double mark);
err_code_e insert(tree_node_t **root, student_t *student, int (*comparator)(student_t *, student_t *));
tree_node_t* find_by_surname(tree_node_t *root, char *surname);
tree_node_t* find_by_mark(tree_node_t *root, double mark);
tree_node_t* find_predecessor(tree_node_t *tree, tree_node_t *node);
tree_node_t* get_max_node(tree_node_t *tree);
void remove_node(tree_node_t **tree, tree_node_t *node);
void print_node(tree_node_t *node);
void free_student(student_t **student);
void free_tree(tree_node_t **root);
void print_node(tree_node_t *node);
void infix_traversal(tree_node_t *root);
void prefix_traversal(tree_node_t *root);
void postfix_traversal(tree_node_t *root);
void export_node(tree_node_t *node, FILE *f);
err_code_e export_tree_to_dot(tree_node_t *root, const char *dot_filename, const char *output_filename);

#endif /* TREE */
