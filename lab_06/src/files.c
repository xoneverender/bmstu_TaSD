#include "files.h"

err_code_e read_data_from_file(char *filename, tree_node_t **tree)
{
    FILE *f;
    student_t *student = NULL;
    err_code_e rc;
    
    if ((f = fopen(filename, "r")) == NULL)
        return ERR_OPENING_FILE;
    
    while ((rc = read_struct(f, &student)) == ERR_SUCCESS)
    {
        (void)insert(tree, student, strings_compare);
    }

    if (rc != ERR_EOF_REACHED)
    {
        fclose(f);
        free_tree(tree);
        return rc;
    }

    fclose(f);

    return ERR_SUCCESS;
}

err_code_e read_struct(FILE *f, student_t **student)
{
    err_code_e rc;
    char *buffer = NULL;

    *student = malloc(sizeof(student_t));
    if (*student == NULL)
        return ERR_ALLOCATING_MEMORY;

    // Чтение фамилии
    if ((rc = read_string(f, &buffer)) != ERR_SUCCESS)
    {
        free(*student);
        return rc == ERR_DATA ? ERR_EOF_REACHED : rc;
    }
    (*student)->surname = buffer;

    // Чтение года
    if ((rc = read_string(f, &buffer)) != ERR_SUCCESS)
    {
        free((*student)->surname);
        free(*student);
        return rc; 
    }

    if (!is_double(&(*student)->average_mark, buffer) || (*student)->average_mark <= 0)
    {
        free((*student)->surname);
        free(buffer);
        free(*student);
        return ERR_NOT_A_NUMBER;
    }
    free(buffer);

    return ERR_SUCCESS;
}
