#ifndef DEFINE_H
#define DEFINE_H
#include "freertos.h"

#define DEVICE_ID 0 // max 32
#define RELAY_OFF 1
#define RELAY_ON 0

#define MAXRCVLEN 1000
#define MAX_MESSAGE_LENGHT 128
#define STRING_DELIMITER ":"

#define DHTPIN 4
#define RELAY_HEAT 23
#define RELAY_LIGHT 22
#define RELAY_FAN 21
#define BUTTON_LIGHT 19
#define BUTTON_UP 18
#define BUTTON_DOWN 5

#define DHTTYPE DHT11 // DHT 11

typedef enum
{
    UNKNOWN_MAIN,
    SENSOR,
    CONTROL,
    BUTTON,
    SYSTEM,
    WIFI_MAIN_LENGHT
} wifi_main_t;
static const char *WIFI_MAIN_TYPE_STRING[] = {"UNKNOWN",
                                              "SENSOR",
                                              "CONTROL",
                                              "BUTTON",
                                              "SYSTEM"};

typedef enum
{
    UNKNOWN_OPERATOR,
    SET,
    GET,
    WIFI_OPERATOR_LENGHT
} wifi_operator_t;
static const char *WIFI_OPERATOR_TYPE_STRING[] = {"UNKNOWN",
                                                  "SET",
                                                  "GET"};

typedef enum
{
    UNKNOWN_TYPE,
    TEMP,
    LIGHT,
    WIFI_TYPE_LENGHT
} wifi_type_t;
static const char *WIFI_TYPE_TYPE_STRING[] = {"UNKNOWN",
                                              "TEMP",
                                              "LIGHT"};

typedef enum
{
    UNKNOWN_SYSTEM,
    RESET,
    START,
    RESTART,
    STOP,
    STATUS_DEVICE,
    GET_ID,
    WIFI_SYSTEM_LENGHT
} wifi_system_t;
static const char *WIFI_SYSTEM_TYPE_STRING[] = {"UNKNOWN",
                                                "RESET",
                                                "START",
                                                "RESTART",
                                                "STOP",
                                                "STATUS",
                                                "ID"};
#endif