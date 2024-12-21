#include "str.h"

err_code_e read_string(FILE *f, char **buffer) 
{   
    ssize_t nread;
    size_t buf_len = 0;
    
    if (*buffer != NULL)
        *buffer = NULL;

    nread = getline(buffer, &buf_len, f);

    if (nread == -1) 
    {
        free(*buffer);

        if (feof(f)) 
            return ERR_EOF_REACHED;
        return ERR_DATA;
    }

    (*buffer)[strcspn(*buffer, "\n")] = '\0';

    if (strlen(*buffer) == 0)
    {
        free(*buffer);
        return ERR_DATA;
    }

    *buffer = realloc(*buffer, (strlen(*buffer) + 1) * sizeof(char));
    
    return ERR_SUCCESS;
}

bool is_long_int(long *num, char *buffer)
{
    char *endptr;    
    
    *num = strtol(buffer, &endptr, 10);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}

bool is_double(double *num, char *buffer)
{
    char *endptr;    
    
    *num = strtod(buffer, &endptr);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}
