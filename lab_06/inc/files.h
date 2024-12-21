#ifndef FILES
#define FILES

#include <stdio.h>
#include <stdbool.h>
#include "errors.h"
#include "str.h"
#include "tree.h"

err_code_e read_data_from_file(char *filename, tree_node_t **tree);
err_code_e read_struct(FILE *f, student_t **record);

#endif /* FILES */
