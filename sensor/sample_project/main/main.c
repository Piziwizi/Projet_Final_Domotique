#include <stdio.h>
#include "wifi.c"
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_wps.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXRCVLEN 500
#define MAX_MESSAGE_LENGHT 128
#define STRING_DELIMITER ":"

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
#define WIFI_MAIN_LENGHT 4

typedef enum
{
    SET,
    GET,
} wifi_operator_t;
static const char *WIFI_OPERATOR_TYPE_STRING[] = {"SET",
                                                  "GET"};
#define WIFI_OPERATOR_LENGHT 2

typedef enum
{
    TEMP,
    LIGHT,
} wifi_type_t;
static const char *WIFI_TYPE_TYPE_STRING[] = {"TEMP",
                                              "LIGHT"};
#define WIFI_TYPE_LENGHT 2

typedef enum
{
    PASSWORD,
    SSID,
    APPLY
} wifi_auth_t;
static const char *WIFI_AUTH_TYPE_STRING[] = {"PASSWORD",
                                              "SSID",
                                              "APPLY"};
#define WIFI_AUTH_LENGHT 3

typedef enum
{
    RESET,
    START,
    RESTART,
    STOP,
    WIFI,
    STATUS_DEVICE
} wifi_system_t;
static const char *WIFI_SYSTEM_TYPE_STRING[] = {"RESET",
                                                "START",
                                                "RESTART",
                                                "STOP",
                                                "WIFI",
                                                "STATUS"};
#define WIFI_SYSTEM_LENGHT 6

uint32_t decode_message(char *msg);

void app_main(void)
{
    init_wifi();
    while (1)
    {
        sleep(30);
        char buffer[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
        int len, mysocket;
        struct sockaddr_in dest;
        printf("test\n");
        mysocket = socket(AF_INET, SOCK_STREAM, 0);

        memset(&dest, 0, sizeof(dest)); /* zero the struct */
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = inet_addr("192.168.0.243"); /* set destination IP number - localhost, 127.0.0.1*/
        dest.sin_port = htons(3333);                       /* set destination port number */
        connect(mysocket, (struct sockaddr *)&dest, sizeof(struct sockaddr_in));

        len = recv(mysocket, buffer, MAXRCVLEN, 0);

        /* We have to null terminate the received data ourselves */
        buffer[len] = '\0';

        printf("Received %s (%d bytes).\n", buffer, len);
        decode_message(buffer);
        close(mysocket);

        printf("test\n");
    }
}

uint32_t decode_message(char *msg)
{
    char *token = strtok(msg, STRING_DELIMITER);
    wifi_main_t main_command = 0;
    wifi_type_t type_command = 0;
    wifi_operator_t op_command = 0;
    wifi_system_t system_command = 0;
    wifi_auth_t auth_command = 0;
    int i = 0;

    while (strcmp(token, WIFI_MAIN_TYPE_STRING[i]) != 0)
    {
        i++;
        if (i > WIFI_MAIN_LENGHT)
        {
            return 0;
        }
        main_command = i;
    }

    switch (main_command)
    {
    case SENSOR:
        token = strtok(NULL, STRING_DELIMITER);
        while (strcmp(token, WIFI_TYPE_TYPE_STRING[i]) != 0)
        {
            i++;
            if (i > WIFI_TYPE_LENGHT)
            {
                return 0;
            }
            type_command = i;
        }
        switch (type_command)
        {
        case TEMP:
            token = strtok(NULL, STRING_DELIMITER);
            while (strcmp(token, WIFI_OPERATOR_TYPE_STRING[i]) != 0)
            {
                i++;
                if (i > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = i;
            }
            switch (op_command)
            {
            case SET:
                //todo ajouter le temp
                printf("%s:%s:%s", WIFI_MAIN_TYPE_STRING[main_command], WIFI_TYPE_TYPE_STRING[type_command], WIFI_OPERATOR_TYPE_STRING[op_command]);
                break;
            case GET:
                //unused
                break;
            default:
                return 0;
            }

            break;
        case LIGHT:
            token = strtok(NULL, STRING_DELIMITER);
            while (strcmp(token, WIFI_OPERATOR_TYPE_STRING[i]) != 0)
            {
                i++;
                if (i > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = i;
            }
            switch (op_command)
            {
            case SET:
                //todo ajouter le light
                break;
            case GET:
                //unused
                break;
            default:
                return 0;
            }
            break;
        default:
            return 0;
        }
        break;

    case CONTROL:
        token = strtok(NULL, STRING_DELIMITER);
        while (strcmp(token, WIFI_TYPE_TYPE_STRING[i]) != 0)
        {
            i++;
            if (i > WIFI_TYPE_LENGHT)
            {
                return 0;
            }
            type_command = i;
        }
        switch (type_command)
        {
        case TEMP:
            token = strtok(NULL, STRING_DELIMITER);
            while (strcmp(token, WIFI_OPERATOR_TYPE_STRING[i]) != 0)
            {
                i++;
                if (i > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = i;
            }
            switch (op_command)
            {
            case SET:
                //todo ajouter le temp
                break;
            case GET:
                //todo ajouter le temp
                break;
            default:
                return 0;
            }
            break;
        case LIGHT:
            token = strtok(NULL, STRING_DELIMITER);
            while (strcmp(token, WIFI_OPERATOR_TYPE_STRING[i]) != 0)
            {
                i++;
                if (i > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = i;
            }
            switch (op_command)
            {
            case SET:
                //todo ajouter le light
                break;
            case GET:
                //todo ajouter le light
                break;
            default:
                return 0;
            }
            break;
        default:
            return 0;
        }
        break;

    case REFRESH:
        token = strtok(NULL, STRING_DELIMITER);
        while (strcmp(token, WIFI_OPERATOR_TYPE_STRING[i]) != 0)
        {
            i++;
            if (i > WIFI_OPERATOR_LENGHT)
            {
                return 0;
            }
            op_command = i;
        }
        switch (op_command)
        {
        case SET:
            //todo ajouter le refesh
            break;
        case GET:
            //todo ajouter le refesh
            break;
        default:
            return 0;
        }
        break;

    case SYSTEM:
        token = strtok(NULL, STRING_DELIMITER);
        while (strcmp(token, WIFI_SYSTEM_TYPE_STRING[i]) != 0)
        {
            i++;
            if (i > WIFI_SYSTEM_LENGHT)
            {
                return 0;
            }
            system_command = i;
        }
        switch (system_command)
        {
        case RESET:
            //todo add reset
            break;
        case START:
            //todo add start
            break;
        case RESTART:
            //todo add restart
            break;
        case STOP:
            //todo add stop
            break;
        case WIFI:
            token = strtok(NULL, STRING_DELIMITER);
            while (strcmp(token, WIFI_AUTH_TYPE_STRING[i]) != 0)
            {
                i++;
                if (i > WIFI_AUTH_LENGHT)
                {
                    return 0;
                }
                auth_command = i;
            }
            switch (auth_command)
            {
            case PASSWORD:
                //todo ajouter la modification password
                break;
            case SSID:
                //todo ajouter le modification du nom
                break;
            case APPLY:
                //todo ajouter le apply
                break;
            default:
                return 0;
            }
            break;
        case STATUS_DEVICE:
            //todo add status
            break;
        default:
            return 0;
        }
        break;
    default:
        return 0;
    }
    return 0;
}
