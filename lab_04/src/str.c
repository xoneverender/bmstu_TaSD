#include "str.h"

err_code_e read_string(FILE *stream, char **buf, size_t *len) 
{   
    ssize_t nread;

    if (*buf != NULL)
    {
        free(*buf);
        *buf = NULL;
    }

    nread = getline(buf, len, stream);

    if (nread == -1) 
    {
        if (feof(stream)) 
            return ERR_EOF_REACHED;

        return ERR_INVALID_INPUT;
    }

    (*buf)[strcspn(*buf, "\n")] = '\0';

    return ERR_SUCCESS;
}

bool is_whole_number(long *num, char *buffer)
{
    char *endptr;    
    
    *num = strtol(buffer, &endptr, 10);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}

bool is_float(float *num, char *buffer)
{
    char *endptr;    
    
    *num = strtod(buffer, &endptr);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}
