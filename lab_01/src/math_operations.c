#include "math_operations.h"

// Функция для умножения двух чисел
void product(number_t *num1, number_t *num2, number_t *res)
{
    int transfer = 0;
    bool overfloat = false;

    // Основной цикл умножения: каждая цифра num2 умножается на каждую цифру num1
    for (int i = 0; i < num2->len; i++)
    {
        for (int j = 0; j < num1->len; j++)
        {
            res->mantissa[j + i] += num1->mantissa[j] * num2->mantissa[i] + transfer;
            // Обновляем перенос для следующего разряда
            transfer = res->mantissa[j + i] / 10;
            // Оставляем в текущем разряде только младшую цифру
            res->mantissa[j + i] %= 10;

        }
        // Обрабатываем возможный перенос для старших разрядов
        res->mantissa[i + num1->len] += transfer;
        if (transfer)
            overfloat = true;
        transfer = 0;
    }

    // Устанавливаем длину результата
    res->len = num1->len + num2->len;

    // Если нет переполнения, уменьшаем длину результата на 1
    if (!overfloat)
        res->len -= 1;

    // Определяем знак и порядок результата
    res->sign = (num1->sign == num2->sign) ? '+' : '-';
    res->exp = num1->exp + num2->exp;
}

// Функция для округления числа
void rounding(number_t *num)
{
    // Индекс 41-й цифры в числе в нормальном виде
    int diff = num->len - 41, i = diff;

    // Если 41-я цифра >= 5, увеличиваем 40-ую цифру на 1
    if (num->mantissa[i++] >= 5)
    {
        for (; i < MAX_RES_SIZE; i++)
        {
            num->mantissa[i]++; 
            // Если переполнения не произошло, выходим из цикла
            if (num->mantissa[i] / 10 == 0)
                break;

            num->mantissa[i] = 0;
        }

        // Если старший разряд перенесся по индексу num->len, значит произошло переполнение
        if (num->mantissa[num->len] != 0)
        {
            num->exp ++;
            diff++;
        }
    }

    num->exp += num->len;

    // Сдвигаем мантиссу для сохранения только первых 40 значащих цифр
    for (i = 0; i < 40; i++)
        num->mantissa[i] = num->mantissa[i + diff + 1];
}

// Функция для получения количества цифр в числе
int count_digits_num(char *string)
{
    int len = 0;
    len = (strchr(string, '+')) ? -1 : 0;
    len += (strchr(string, '-')) ? -1: 0;
    len += (strchr(string, '.')) ? -1: 0;
    len += strlen(string);
    return len;
}

int count_el_in_string(char *string, char el)
{
    int count = 0;
    for (size_t i = 0; i < strlen(string); i++)
        count += string[i] == el;
    return count;
}
