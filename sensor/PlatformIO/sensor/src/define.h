#ifndef DEFINE_H
#define DEFINE_H
#include "freertos.h"

#define ENABLE_HEAT_SENSOR 1
#define ENABLE_LIGHT_SENSOR 0
#define ENABLE_HEAT_CONTROL 0
#define ENABLE_LIGHT_CONTROL 0
#define DEVICE_ID 0 // max 32

#define MAXRCVLEN 1000
#define MAX_MESSAGE_LENGHT 128
#define STRING_DELIMITER ":"

typedef enum
{
    UNKNOWN_MAIN,
    SENSOR,
    CONTROL,
    REFRESH,
    SYSTEM,
} wifi_main_t;
static const char *WIFI_MAIN_TYPE_STRING[] = {"UNKNOWN",
                                              "SENSOR",
                                              "CONTROL",
                                              "REFRESH",
                                              "SYSTEM"};
#define WIFI_MAIN_LENGHT 5

typedef enum
{
    UNKNOWN_OPERATOR,
    SET,
    GET,
} wifi_operator_t;
static const char *WIFI_OPERATOR_TYPE_STRING[] = {"UNKNOWN",
                                                  "SET",
                                                  "GET"};
#define WIFI_OPERATOR_LENGHT 3

typedef enum
{
    UNKNOWN_TYPE,
    TEMP,
    LIGHT,
} wifi_type_t;
static const char *WIFI_TYPE_TYPE_STRING[] = {"UNKNOWN",
                                              "TEMP",
                                              "LIGHT"};
#define WIFI_TYPE_LENGHT 3

typedef enum
{
    UNKNOWN_AUTH,
    PASSWORD,
    SSID,
    APPLY
} wifi_auth_t;
static const char *WIFI_AUTH_TYPE_STRING[] = {"UNKNOWN",
                                              "PASSWORD",
                                              "SSID",
                                              "APPLY"};
#define WIFI_AUTH_LENGHT 4

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
static const char *WIFI_SYSTEM_TYPE_STRING[] = {"UNKNOWN",
                                                "RESET",
                                                "START",
                                                "RESTART",
                                                "STOP",
                                                "WIFI",
                                                "STATUS",
                                                "ID"};
#define WIFI_SYSTEM_LENGHT 8

SemaphoreHandle_t mutex_read_temp, mutex_read_light, mutex_heat, mutex_light;

float read_temp, read_light = 0;
bool heat, light = 0;

#endif