#include "user_interface.h"

int read_line(char *buffer, size_t buffer_size, size_t MAX_SIZE)
{
    printf("Введите число, содержащее до %zu значащих цифр в мантиссе и до %d в порядке: \n", MAX_SIZE, MAX_EXP_SIZE);
    
    if (MAX_SIZE == MAX_MANTISSA_SIZE1)
        puts(">+----~----1----~----2----~----3----~");
    else
        puts(">+----~----1----~----2----~----3----~----4");
    printf(">");

    if (!fgets(buffer, buffer_size, stdin))
        return ERR_EMPTY_LINE;

    buffer[strcspn(buffer, "\n")] = '\0';
    
    return ERR_SUCCESS;
}

void show_program_purpose(void)
{
    puts("Данная программа умножает два действительных числа в форме ±m.nЕ±K");
}

void print_result(number_t *res)
{
    puts("Результат умножения (в форме ±0.mЕ±K, мантисса содержит до 40 значащих цифр, порядок - до 5 значащих цифр): ");

    printf("%c0.", res->sign);

    // длина меняется не везде ?????
    for (size_t i = res->len; i > 0; i--)
        printf("%d", res->mantissa[i - 1]);

    printf("e%d\n", res->exp);
}
