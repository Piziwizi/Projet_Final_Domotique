#ifndef DEFINE_H
#define DEFINE_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include "json-c/json.h"
#include "json-c/json_object.h"
#include "json-c/json_tokener.h"
#include "json-c/arraylist.h"
#include "json-c/json_util.h"

#define SENSOR_FILE "../../Interface/sensor.json"
#define CONTROL_FILE "../../Interface/control.json"
#define MAX_SENSORS 32
#define AVAILABLE 1
#define USED 0

typedef enum {TEMP,LIGHT} sensor_type_t;
static const char *SENSOR_TYPE_STRING[] = {"temperature", "light"};

typedef struct{
    uint32_t id;
    sensor_type_t type;
    double value;
}sensor_t;

typedef struct{
    sensor_t* tab[MAX_SENSORS];
    uint32_t available[MAX_SENSORS];
}sensor_tab_t;

pthread_mutex_t mutex_sensor;
char* sensor_string;

pthread_mutex_t mutex_control;

pthread_t thread_sensor;
pthread_t thread_sensor_manager;
pthread_t thread_control;
pthread_t thread_control_manager;
pthread_t thread_interface;
pthread_t thread_logging;

sensor_tab_t sensor_tab;

#endif