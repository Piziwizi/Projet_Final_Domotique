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

#define SENSOR_FILE "../../Interface/sensor.json"
#define CONTROL_FILE "../../Interface/control.json"
#define MAX_SENSORS 32
#define MAX_CONTROLS 32
#define MAX_CHAR_FILE 2048
#define AVAILABLE 1
#define USED 0

//times
#define REFRESH_PERIOD_INTERFACE 5
#define REFRESH_PERIOD_SENSOR 20
#define REFRESH_PERIOD_SEARCH_SENSOR 10
#define REFRESH_PERIOD_CONTROL 20
#define REFRESH_PERIOD_SEARCH_CONTROL 60
#define TIMEOUT_CYCLE_REMOVE_SENSOR 10
#define HYSTERESIS_VALUE 0.3

#define NUMBER_OF_MODULE_TYPES 2
typedef enum
{
    TEMP,
    LIGHT
} module_type_t;
static const char *MODULE_TYPE_STRING[] = {"temperature", "light"};

typedef enum
{
    REFRESH_SENSORS,
    SEARCH_NEW_SENSORS,
    ADD_SENSOR,
    REMOVE_SENSOR,
    REMOVE_ALL_SENSORS,
    TO_INTERFACE,
    FROM_INTERFACE,
    IDLE,
    EXIT
} sensor_state_machine_t;

typedef enum
{
    UNKNOWN_MAIN,
    SENSOR,
    CONTROL,
    REFRESH,
    SYSTEM
} wifi_main_t;
static const char *WIFI_MAIN_TYPE_STRING[] = {"SENSOR",
                                              "CONTROL",
                                              "REFRESH",
                                              "SYSTEM"};

typedef enum
{
    UNKNOWN_OPERATOR,
    SET,
    GET,
} wifi_operator_t;
static const char *WIFI_OPERATOR_TYPE_STRING[] = {"SET",
                                                  "GET"};

typedef enum
{
    UNKNOWN_TYPE,
    TEMP_DEVICE,
    LIGHT_DEVICE,
} wifi_type_t;
static const char *WIFI_TYPE_TYPE_STRING[] = {"TEMP",
                                              "LIGHT"};

typedef enum
{
    UNKNOWN_AUTH,
    PASSWORD,
    SSID,
    APPLY
} wifi_auth_t;
static const char *WIFI_AUTH_TYPE_STRING[] = {"PASSWORD",
                                              "SSID",
                                              "APPLY"};

typedef enum
{
    UNKNOWN_SYSTEM,
    RESET,
    START,
    RESTART,
    STOP,
    WIFI,
    STATUS_DEVICE,
    GET_ID
} wifi_system_t;
static const char *WIFI_SYSTEM_TYPE_STRING[] = {"RESET",
                                                "START",
                                                "RESTART",
                                                "STOP",
                                                "WIFI",
                                                "STATUS"};

typedef struct
{
    uint32_t id;
    module_type_t type;
    double value;
} sensor_t;

typedef struct
{
    sensor_t *tab[MAX_SENSORS];
    uint32_t available[MAX_SENSORS];
    sem_t sensor_sem_tab[MAX_SENSORS];
    pthread_t thread_sensor_tab[MAX_SENSORS];
    in_addr_t ip[MAX_SENSORS];
} sensor_tab_t;

typedef struct
{
    uint32_t id;
    module_type_t type;
    double value;
} control_t;

typedef struct
{
    control_t *tab[MAX_SENSORS];
    uint32_t available[MAX_SENSORS];
    sem_t control_sem_tab[MAX_SENSORS];
    pthread_t thread_control_tab[MAX_SENSORS];
    in_addr_t ip[MAX_SENSORS];
} control_tab_t;

pthread_mutex_t mutex_sensor_interface;
pthread_mutex_t mutex_sensor_tab;
char *sensor_string;

pthread_mutex_t mutex_control_interface;
pthread_mutex_t mutex_control_tab;
char control_string[MAX_CHAR_FILE];

pthread_mutex_t mutex_log;

pthread_t thread_refresh_sensor;
pthread_t thread_search_sensor;
pthread_t thread_save_sensor;

pthread_t thread_refresh_control;
pthread_t thread_search_control;
pthread_t thread_read_control;

pthread_t thread_control;
pthread_t thread_control_manager;
pthread_t thread_interface;
pthread_t thread_logging;
pthread_t thread_timming;

pthread_t thread_test;

sensor_tab_t sensor_tab;
control_tab_t control_tab;

#endif