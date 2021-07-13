#include "wifi_com.h"
#include <WiFi.h>
#include <WiFiMulti.h>

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
    STATUS_DEVICE
} wifi_system_t;
static const char *WIFI_SYSTEM_TYPE_STRING[] = {"UNKNOWN",
                                                "RESET",
                                                "START",
                                                "RESTART",
                                                "STOP",
                                                "WIFI",
                                                "STATUS"};
#define WIFI_SYSTEM_LENGHT 7

void wifi_client(void *parameter);
uint32_t decode_message(String msg);

void init_wifi(void){
  xTaskCreate(
      wifi_client,   // Function that should be called
      "Wifi Client", // Name of the task (for debugging)
      5000,          // Stack size (bytes)
      NULL,          // Parameter to pass
      1,             // Task priority
      NULL           // Task handle
  );
}

void wifi_client(void *parameter)
{
  WiFiMulti WiFiMulti;

  WiFiMulti.addAP("HOMETS", "VerySecurePassword3!");

  while (WiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  for (;;)
  {
    const uint16_t port = 3333;
    const char *host = "10.10.10.10"; // ip or dns

    Serial.print("Connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;

    if (!client.connect(host, port))
    {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      vTaskDelay(5000 / portTICK_PERIOD_MS);
      return;
    }

    // This will send a request to the server
    //uncomment this line to send an arbitrary string to the server
    //client.print("Send this data to the server");
    //uncomment this line to send a basic document request to the server

    int maxloops = 0;

    //wait for the server's reply to become available
    while (!client.available() && maxloops < 1000)
    {
      maxloops++;
      vTaskDelay(20 / portTICK_PERIOD_MS);
    }
    if (client.available() > 0)
    {
      //read back one line from the server
      String line = client.readStringUntil('\r');
      Serial.println(line);
      decode_message(line);
    }
    else
    {
      Serial.println("client.available() timed out ");
    }

    Serial.println("Closing connection.");
    client.stop();

    Serial.println("Waiting 5 seconds before restarting...");
    vTaskDelay(5000 / portTICK_PERIOD_MS);
  }
}

uint32_t decode_message(String msg)
{
    String sub_msg = msg;

    int current_index = sub_msg.indexOf(STRING_DELIMITER);
    String token = sub_msg.substring(0, current_index);
    sub_msg = sub_msg.substring(current_index+1);

    wifi_main_t main_command = UNKNOWN_MAIN;
    wifi_type_t type_command = UNKNOWN_TYPE;
    wifi_operator_t op_command = UNKNOWN_OPERATOR;
    wifi_system_t system_command = UNKNOWN_SYSTEM;
    wifi_auth_t auth_command = UNKNOWN_AUTH;
    int i = 0;
    int j = 0;
    int k = 0;
    
    while (token.compareTo(WIFI_MAIN_TYPE_STRING[i]) != 0)
    {
        i++;
        if (i > WIFI_MAIN_LENGHT)
        {
            return 0;
        }
        main_command = (wifi_main_t) i;
    }
    
    switch (main_command)
    {
    case SENSOR:
        current_index = sub_msg.indexOf(STRING_DELIMITER);
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index+1);
        
        while (token.compareTo(WIFI_TYPE_TYPE_STRING[j]) != 0)
        {
            j++;
            if (j > WIFI_TYPE_LENGHT)
            {  
                return 0;
            }
            type_command = (wifi_type_t) j;
        }
        switch (type_command)
        {
        case TEMP:
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index+1);

            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                if (k > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t) k;
            }
            switch (op_command)
            {
            case SET:
                //unused
                break;
            case GET:
                //todo ajouter light
                break;
            default:
                return 0;
            }

            break;
        case LIGHT:
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index+1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                if (k > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command =  (wifi_operator_t) k;
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
        current_index = sub_msg.indexOf(STRING_DELIMITER);
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index+1);
        while (token.compareTo(WIFI_TYPE_TYPE_STRING[j]) != 0)
        {
            j++;
            if (j > WIFI_TYPE_LENGHT)
            {
                return 0;
            }
            type_command = (wifi_type_t) j;
        }
        switch (type_command)
        {
        case TEMP:
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index+1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                if (k > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t) k;
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
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index+1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                if (k > WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t)k;
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
        current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index+1);
        while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[j]) != 0)
        {
            j++;
            if (j > WIFI_OPERATOR_LENGHT)
            {
                return 0;
            }
            op_command = (wifi_operator_t) j;
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
        current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index+1);
        while (token.compareTo(WIFI_SYSTEM_TYPE_STRING[j]) != 0)
        {
            j++;
            if (j > WIFI_SYSTEM_LENGHT)
            {
                return 0;
            }
            system_command = (wifi_system_t)j;
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
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if(current_index <= 0){
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index+1);
            while (token.compareTo(WIFI_AUTH_TYPE_STRING[k]) != 0)
            {
                k++;
                if (k > WIFI_AUTH_LENGHT)
                {
                    return 0;
                }
                auth_command = (wifi_auth_t) k;
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