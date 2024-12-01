#include "str.h"

bool is_long_int(long *num, char *buffer)
{
    char *endptr, *end;    
    
    // Удаление пробелов в конце строки
    while (buffer && isspace(*buffer))
        buffer++;
            
    // Удаление пробелов в конце строки
    end = buffer + strlen(buffer) - 1;
    while (end > buffer && isspace(*end))
        end--;
    *(end + 1) = '\0';

    *num = strtol(buffer, &endptr, 10);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}

bool is_float(float *num, char *buffer)
{
    char *endptr, *end;    
    
    // Удаление пробелов в конце строки
    while (buffer && isspace(*buffer))
        buffer++;
            
    // Удаление пробелов в конце строки
    end = buffer + strlen(buffer) - 1;
    while (end > buffer && isspace(*end))
        end--;
    *(end + 1) = '\0';
    
    *num = strtod(buffer, &endptr);

    if (*endptr != '\0' || endptr == buffer) 
        return false;

    return true; 
}

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

        return ERR_READING_STRING;
    }

    (*buf)[strcspn(*buf, "\n")] = '\0';

    if (**buf == '\0')
        return ERR_EMPTY_STRING;

    return ERR_SUCCESS;
}

err_code_e extract_token(char **token, char *start, char *end)
{
    int len = end - start;

    *token = malloc(sizeof(char) * (len + 1));
    if (!*token)
        return ERR_ALLOCATING_MEMORY;

    *token = memcpy(*token, start, len);
    (*token)[len] = '\0';

    return ERR_SUCCESS;
}

err_code_e parse_string(char *buf, void *stack, err_code_e (*push_func)(void *, data_t *))
{
    err_code_e rc;
    bool prev_is_int = false;
    char *token_start, *token;
    char temp[2];

    if (!buf)
        return ERR_NULL_PTR;

    do
    {   
        while (isspace(*buf))
            buf++;
            
        if (strchr("+-*/", *buf) == NULL)
        {
            if (!prev_is_int)
            {
                prev_is_int = true;
                token_start = buf;
            }
        }
        else
        {
            if (prev_is_int)
            {
                prev_is_int = false;

                if ((rc = extract_token(&token, token_start, buf)))
                    return rc;

                if ((rc = push(stack, token, push_func)) != ERR_SUCCESS)
                {   
                    free(token);
                    return rc;
                }

                if (*buf)
                {
                    temp[0] = *buf;
                    temp[1] = 0;


                    if ((rc = push(stack, temp, push_func)) != ERR_SUCCESS)
                        return rc;
                }
                else
                    break;
            }
            else
                return ERR_INVALID_STACK_INPUT;

            if (!*buf)
                break;
        }

        buf++;
    } while (1);

    operate_printing_array_stack(stack);

    if (strchr("+-/*", *(buf - 1)))
        return ERR_INVALID_STACK_INPUT;
    
    return ERR_SUCCESS;
}
