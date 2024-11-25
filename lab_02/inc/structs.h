#ifndef STRUCTS
#define STRUCTS

#include <stdint.h>
#include <stdbool.h> 
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <time.h>
#include "errors.h"
#include "consts.h"
#include "str.h"

// Определение перечисления для видов литературы
typedef enum 
{
    TECHNICAL, // Техническая литература
    FICTION,   // Художественная литература
    CHILDREN   // Детская литература
} literature_type_t;

// Определение перечисления для жанров детской литературы
typedef enum 
{
    POEMS,        // Стихи
    FAIRY_TALES   // Сказки
} children_genre_t;

// Структура для детской литературы
typedef struct
{
    uint8_t min_age;           // Минимальный возраст для чтения
    children_genre_t genre;    // Жанр детской литературы (стихи или сказки)
} children_t;

// Определение перечисления для типов художественной литературы
typedef enum
{
    NOVEL, // Роман
    PLAY,  // Пьеса
    POETRY // Поэзия
} fiction_t;

// Структура для технической литературы
typedef struct 
{
    char branch[MAX_STRING_LEN + 1];            // Отрасль (например, "Информатика")
    bool is_translated;         // Флаг: true - переводная, false - отечественная
    uint16_t year_of_publication; // Год издания
} technical_t;

// Основная структура для книги
typedef struct
{
    char author[MAX_STRING_LEN + 1];            // Фамилия автора
    char title[MAX_STRING_LEN + 1];             // Название книги
    char publisher[MAX_STRING_LEN + 1];         // Название издательства
    uint16_t pages;             // Количество страниц в книге
    literature_type_t type;     // Вид литературы (техническая, художественная, детская)
    union
    {
        children_t children;     // Данные для детской литературы
        fiction_t fiction;       // Данные для художественной литературы
        technical_t technical;   // Данные для технической литературы
    } type_t; 
} book_t;

typedef struct
{
    uint16_t index;
    char title[MAX_STRING_LEN + 1];
} key_t;

err_code read_struct(FILE *f, char *buf, book_t *record);
bool is_literature_type(const char *buf, literature_type_t *type);
bool is_fiction_type(const char *buf, fiction_t *type);
bool is_children_type(const char *buf, children_genre_t *type);
bool is_boolean(const char *buf, bool *boolean);
int compare_books(const void *book1, const void *book2);
int compare_keys(const void *key1, const void *key2);
void bubblesort(void *base, size_t num, size_t size, int (*compare)(const void *, const void *));
void swap(void *a, void *b, size_t size);
void shuffle(void *array, const int num_elements, const size_t size);
void create_keys_arr(key_t *keys, book_t *records, const int records_num);

#endif /* STRUCTS */
