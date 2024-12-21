#define _GNU_SOURCE
#ifndef ACTIONS_PROCESSING
#define ACTIONS_PROCESSING

#include <stdio.h>
#include <float.h>
#include <time.h>
#include <sys/time.h>
#include "errors.h"
#include "colors.h"
#include "io.h"
#include "str.h"
#include "tree.h"
#include "files.h"


#define MIN_ELEMENTS 1000
#define MAX_ELEMENTS 5000
#define STEP 1000
#define ITERATIONS 100

typedef enum
{
    INORDER = 1,
    PREORDER = 2,
    POSTORDER = 3,
    GRAPHICAL = 4
} tree_output_e;

typedef enum
{
    EXIT,
    ADD_STUDENT,
    REMOVE_STUDENT,
    REMOVE_STUDENTS_WITH_BAD_MARKS,
    FIND_STUDENT,
    UPLOAD_TREE_DATA,
    SHOW_TREE,
    COMPARE_ALGS
} action_e;

err_code_e process_action(action_e action, tree_node_t **root);
err_code_e operate_adding_student(tree_node_t **root);
err_code_e operate_removing_student(tree_node_t **root);
err_code_e operate_removing_low_mark_nodes(tree_node_t **root);
err_code_e operate_finding_student_info(tree_node_t **root);
err_code_e operate_uploading_data_from_file(tree_node_t **root);
err_code_e operate_printing_tree(tree_node_t *root);

void operate_comparizon(void);
unsigned long long us_now(void);
student_t **generate_students(size_t count);
char *generate_random_surname(size_t length);
student_t **deep_copy_array(size_t count, student_t **src);

#endif /* ACTIONS_PROCESSING */

