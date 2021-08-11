#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "json-c/json.h"
#include "json-c/json_object.h"
#include "json-c/json_tokener.h"
#include "json-c/arraylist.h"
#include "json-c/json_util.h"
#include "logging.h"
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define SENSOR_FILE "../../Interface/static/sensor.json"
#define CONTROL_FILE "../../Interface/static/control.json"
#define MAX_SENSORS 32
#define MAX_CONTROLS MAX_SENSORS * 2
#define MAX_CHAR_FILE 2048
#define AVAILABLE 1
#define USED 0

//times
#define REFRESH_PERIOD_INTERFACE 0
#define REFRESH_PERIOD_SEARCH_SENSOR 3
#define TIMEOUT_CYCLE_REMOVE_SENSOR 10
#define HYSTERESIS_VALUE 0.3

/**
 * @name module_type_t
 * @brief The type for all modules types.
 */
typedef enum
{
    TEMP,
    LIGHT,
    NUMBER_OF_MODULE_TYPES
} module_type_t;
static const char *MODULE_TYPE_STRING[] = {"temperature", "light"};

/**
 * @name sensor_t
 * @brief Single sensor.
 */
typedef struct
{
    uint32_t id;
    module_type_t type;
    double value;
} sensor_t;

/**
 * @name sensor_tab_t
 * @brief All sensor data structure.
 */
typedef struct
{
    sensor_t *tab[MAX_SENSORS];
    uint32_t available[MAX_SENSORS];
    sem_t sensor_sem_tab[MAX_SENSORS];
    pthread_t thread_sensor_tab[MAX_SENSORS];
    in_addr_t ip[MAX_SENSORS];
} sensor_tab_t;

/**
 * @name control_t
 * @brief Single control.
 */
typedef struct
{
    uint32_t id;
    module_type_t type;
    double value;
} control_t;

/**
 * @name control_tab_t
 * @brief All control data structure.
 */
typedef struct
{
    control_t *tab[MAX_CONTROLS];
    uint32_t available[MAX_CONTROLS];
} control_tab_t;

pthread_mutex_t mutex_sensor_interface;
pthread_mutex_t mutex_sensor_tab;
char sensor_string[MAX_CHAR_FILE];

pthread_mutex_t mutex_control_interface;
pthread_mutex_t mutex_control_tab;
char control_string[MAX_CHAR_FILE];
char control_string_write[MAX_CHAR_FILE];
sem_t control_sem_save;

pthread_mutex_t mutex_log;

pthread_t thread_search_sensor;
pthread_t thread_save_sensor;
pthread_t thread_save_control;
pthread_t thread_read_control;
pthread_t thread_interface;
pthread_t thread_logging;

pthread_t thread_test;

sensor_tab_t sensor_tab;
control_tab_t control_tab;
uint32_t writing_control;

#endif