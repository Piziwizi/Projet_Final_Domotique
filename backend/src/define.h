#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "json-c/json.h"
#include "json-c/json_object.h"
#include "json-c/json_tokener.h"
#include "json-c/arraylist.h"
#include "json-c/json_util.h"

#define SENSOR_FILE "../../Interface/sensor.json"
#define CONTROL_FILE "../../Interface/control.json"
#define MAX_SENSORS 32
#define MAX_CHAR_FILE 2048
#define AVAILABLE 1
#define USED 0

typedef enum
{
    TEMP,
    LIGHT
} sensor_type_t;
static const char *SENSOR_TYPE_STRING[] = {"temperature", "light"};

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
    SET,
    GET,
} wifi_operator_t;
static const char *WIFI_OPERATOR_TYPE_STRING[] = {"SET",
                                                  "GET"};

typedef enum
{
    TEMP_DEVICE,
    LIGHT_DEVICE,
} wifi_type_t;
static const char *WIFI_TYPE_TYPE_STRING[] = {"TEMP",
                                              "LIGHT"};

typedef enum
{
    PASSWORD,
    SSID,
    APPLY
} wifi_auth_t;
static const char *WIFI_AUTH_TYPE_STRING[] = {"PASSWORD",
                                              "SSID",
                                              "APPLY"};

typedef enum
{
    RESET,
    START,
    RESTART,
    STOP,
    WIFI,
    STATUS
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
    sensor_type_t type;
    double value;
} sensor_t;

typedef struct
{
    sensor_t *tab[MAX_SENSORS];
    uint32_t available[MAX_SENSORS];
    sem_t sensor_sem_tab[MAX_SENSORS];
    pthread_t thread_sensor_tab[MAX_SENSORS];
} sensor_tab_t;

pthread_mutex_t mutex_sensor;
char *sensor_string;

pthread_mutex_t mutex_control;
char control_string[MAX_CHAR_FILE];

pthread_t thread_sensor_manager;
pthread_t thread_control;
pthread_t thread_control_manager;
pthread_t thread_interface;
pthread_t thread_logging;

pthread_t thread_test;

sensor_tab_t sensor_tab;

#endif