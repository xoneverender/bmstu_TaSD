#ifndef FILES
#define FILES

#include <stdio.h>
#include "errors.h"
#include "matrix.h"
#include "str.h"

err_code open_file(const char *filename);
err_code read_matrix_from_file(const char *filename, matrix_t *matrix);
err_code read_matrix_data(FILE *f, matrix_t *matrix);
err_code read_matrix_dimensions(FILE *f, int *rows, int *columns);

#endif /* FILES */
