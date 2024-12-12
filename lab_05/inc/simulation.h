#ifndef SIMULATION
#define SIMULATION

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <float.h>
#include "errors.h"
#include "queue.h"
#include "io.h"
#include "str.h"

#define MAX_REQUESTS 1000
#define CHECK_POINT 100

typedef struct
{
    request_t *request;
} request_handler_t;

typedef enum
{
    ARRAY = 1,
    LIST = 2
} mode_e;

typedef enum
{
    VISIBLE = 1,
    HIDE = 2
} visibility_e;

typedef enum
{
    CHANGED = 1,
    UNCHANGED = 2
} time_settings_e;

typedef struct
{
    double start;
    double end;
} interval_t;

typedef struct 
{
    interval_t interval_type1;  // Интервал времени между добавлением заявок первого типа
    interval_t interval_type2;  // Интервал времени между добавлением заявок второго типа
    interval_t processing_time1; // Время обработки заявки первого типа
    interval_t processing_time2; // Время обработки заявки второго типа
} timing_data_t;

typedef enum
{
    FIRST = 1,
    SECOND = 2
} request_type_e;

typedef struct 
{
    double next_add_type1;   // Время добавления следующей заявки первого типа в очередь
    double next_add_type2;   // Время добавления следующей заявки второго типа в очередь
    double next_remove;      // Время снятия заявки с процессора
} queue_event_t;

int queue_length(void *queue, mode_e mode);
double get_min_positive_time(queue_event_t events);
void free_all(request_handler_t *generator_1, request_handler_t *generator_2, request_handler_t *processor, void *queue_1, void *queue_2, err_code_e (*free_func)(void *));
void results_for_one_queue(int curr_len, double avg_len, double avg_time_in_queue, int entered, int exited, request_type_e type);
void summary(double simulation_time, double idle_time);
bool is_empty(void *queue, mode_e mode);
err_code_e collect_data_for_simulation(visibility_e *adress_visibility, time_settings_e *time_settings, mode_e *mode);
err_code_e get_time_intervals(timing_data_t *timings, time_settings_e time_settings);
err_code_e set_interval(interval_t *interval);
err_code_e generate_request(request_t **request, timing_data_t timings, request_type_e type, double generation_moment);
err_code_e simulate(void);
void print_queue(void *queue, mode_e mode);
void *get_queue_ptr(void *queue, mode_e mode);

#endif /* SIMULATION */
