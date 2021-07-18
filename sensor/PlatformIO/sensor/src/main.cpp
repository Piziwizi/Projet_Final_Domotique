#include "define.h"
#include "DHT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#define DHTPIN 4
#define RELAY_HEAT 23
#define RELAY_LIGHT 22
#define BUTTON 19

#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
WiFiServer server_device(3334);

//************  déclaration fonction   ************
#if ENABLE_HEAT_SENSOR
float read_temperature(float *Temperature);
#endif
void wifi_client(void *parameter);
void init_wifi(void);
void wifi_client(void *parameter);
void wifi_server(void *parameter);
uint32_t decode_message(String msg, WiFiClient *local);

void setup()
{
#if ENABLE_HEAT_SENSOR
    mutex_read_temp = xSemaphoreCreateMutex();
    if (mutex_read_temp == NULL)
    {
        ESP.restart();
    }
#endif
#if ENABLE_LIGHT_SENSOR
    mutex_read_light = xSemaphoreCreateMutex();
    if (mutex_read_light == NULL)
    {
        ESP.restart();
    }
#endif

#if ENABLE_HEAT_CONTROL
    mutex_heat = xSemaphoreCreateMutex();
    if (mutex_heat == NULL)
    {
        ESP.restart();
    }
    pinMode(RELAY_HEAT, OUTPUT);
#endif
#if ENABLE_LIGHT_CONTROL
    mutex_light = xSemaphoreCreateMutex();
    if (mutex_light == NULL)
    {
        ESP.restart();
    }
    pinMode(RELAY_LIGHT, OUTPUT);
#endif
    pinMode(BUTTON, INPUT); // pour tester

    Serial.begin(115200);
    dht.begin();

    init_wifi();
}

void loop()
{
#if ENABLE_HEAT_SENSOR
    xSemaphoreTake(mutex_read_temp, portMAX_DELAY);
    read_temperature(&read_temp);
    xSemaphoreGive(mutex_read_temp);
    // heat = digitalRead(BUTTON); // pour tester
#endif

#if ENABLE_HEAT_CONTROL
    xSemaphoreTake(mutex_heat, portMAX_DELAY);
    digitalWrite(RELAY_HEAT, heat);
    xSemaphoreGive(mutex_heat);
#endif

#if ENABLE_LIGHT_CONTROL
    xSemaphoreTake(mutex_light, portMAX_DELAY);
    digitalWrite(RELAY_LIGHT, light);
    xSemaphoreGive(mutex_light);
#endif
}
//*************   FONCTION   ***************//

#if ENABLE_HEAT_SENSOR
float read_temperature(float *Temperature) //
{
    *Temperature = dht.readTemperature(); // put value in the global variable
    float temperature = *Temperature;     // to stop accessing the global variable for no reason

    if (isnan(temperature)) // Check if any reads failed and exit early (to try again)
    {
        Serial.println(F("Failed to read from DHT sensor!"));
    }
    else
    {
        Serial.print(F("Temperature: "));
        Serial.print(temperature);
        Serial.println(F("°C "));
    }
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    return temperature;
}
#endif

void init_wifi(void)
{
    //WiFiMulti WiFiMulti;
    WiFi.begin("HOMETS", "VerySecurePassword3!");

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    server_device.begin();

    xTaskCreate(
        wifi_client,   // Function that should be called
        "Wifi Client", // Name of the task (for debugging)
        10000,         // Stack size (bytes)
        NULL,          // Parameter to pass
        1,             // Task priority
        NULL           // Task handle
    );

    xTaskCreate(
        wifi_server,   // Function that should be called
        "Wifi Server", // Name of the task (for debugging)
        10000,         // Stack size (bytes)
        NULL,          // Parameter to pass
        1,             // Task priority
        NULL           // Task handle
    );
}

void wifi_client(void *parameter)
{
    const uint16_t port = 3333;
    const char *host = "10.10.10.10"; // ip or dns

    for (;;)
    {
        Serial.print("Connecting to ");
        Serial.println(host);

        if (!client.connect(host, port))
        {
            Serial.println("Connection failed.");
            Serial.println("Waiting 5 seconds before retrying...");
            vTaskDelay(500 / portTICK_PERIOD_MS);
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
        if (client.available())
        {
            //read back one line from the server
            String line = client.readStringUntil('\r');
            Serial.println(line);
            decode_message(line,&client);
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

void wifi_server(void *parameter)
{
    for (;;)
    {
        WiFiClient pi = server_device.accept();
        if (pi)
        {
            Serial.println("Incomming message");
            while (pi.connected())
            {
                if (pi.available())
                {
                    String line = pi.readStringUntil('\r');
                    Serial.println(line);
                    decode_message(line,&pi);
                }
                vTaskDelay(1/ portTICK_PERIOD_MS);
            }
            pi.stop();
        }
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}

uint32_t decode_message(String msg , WiFiClient * local)
{
    String sub_msg = msg;

    int current_index = sub_msg.indexOf(STRING_DELIMITER);
    String token = sub_msg.substring(0, current_index);
    sub_msg = sub_msg.substring(current_index + 1);

    wifi_main_t main_command = UNKNOWN_MAIN;
    wifi_type_t type_command = UNKNOWN_TYPE;
    wifi_operator_t op_command = UNKNOWN_OPERATOR;
    wifi_system_t system_command = UNKNOWN_SYSTEM;
    wifi_auth_t auth_command = UNKNOWN_AUTH;
    int32_t i = 0;
    int32_t j = 0;
    int32_t k = 0;

    while (token.compareTo(WIFI_MAIN_TYPE_STRING[i]) != 0)
    {
        i++;
        vTaskDelay(1 / portTICK_PERIOD_MS);
        if (i == WIFI_MAIN_LENGHT)
        {
            return 0;
        }
        main_command = (wifi_main_t)i;
    }

    switch (main_command)
    {
    case SENSOR:
        current_index = sub_msg.indexOf(STRING_DELIMITER);
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index + 1);

        while (token.compareTo(WIFI_TYPE_TYPE_STRING[j]) != 0)
        {
            j++;
            vTaskDelay(1 / portTICK_PERIOD_MS);
            if (j == WIFI_TYPE_LENGHT)
            {
                return 0;
            }
            type_command = (wifi_type_t)j;
        }
        switch (type_command)
        {
        case TEMP:
#if ENABLE_HEAT_SENSOR
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if (current_index <= 0)
            {
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index + 1);

            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                if (k == WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t)k;
            }
            switch (op_command)
            {
            case SET:
                //unused
                break;
            case GET:
                Serial.println("Test");
                xSemaphoreTake(mutex_read_temp, portMAX_DELAY);
                local->printf("%f", 20.5);
                //local->printf("%f", read_temp);
                xSemaphoreGive(mutex_read_temp);
                break;
            default:
                return 0;
            }
#endif
            break;
        case LIGHT:
#if ENABLE_LIGHT_SENSOR
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if (current_index <= 0)
            {
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index + 1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                if (k == WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t)k;
            }
            switch (op_command)
            {
            case SET:
                //unused
                break;
            case GET:
                //unused
                break;
            default:
                return 0;
            }
#endif
            break;
        default:
            return 0;
        }
        break;
    case CONTROL:
        current_index = sub_msg.indexOf(STRING_DELIMITER);
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index + 1);
        while (token.compareTo(WIFI_TYPE_TYPE_STRING[j]) != 0)
        {
            j++;
            vTaskDelay(1 / portTICK_PERIOD_MS);
            if (j == WIFI_TYPE_LENGHT)
            {
                return 0;
            }
            type_command = (wifi_type_t)j;
        }
        switch (type_command)
        {
        case TEMP:
#if ENABLE_HEAT_CONTROL
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if (current_index <= 0)
            {
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index + 1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                if (k == WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t)k;
            }
            switch (op_command)
            {
            case SET:
                xSemaphoreTake(mutex_heat, portMAX_DELAY);
                heat = token.toInt();
                xSemaphoreGive(mutex_heat);
                break;
            case GET:
                xSemaphoreTake(mutex_heat, portMAX_DELAY);
                local->printf("%d", heat);
                xSemaphoreGive(mutex_heat);
                break;
            default:
                return 0;
            }
#endif
            break;
        case LIGHT:
#if ENABLE_LIGHT_CONTROL
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if (current_index <= 0)
            {
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index + 1);
            while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[k]) != 0)
            {
                k++;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                if (k == WIFI_OPERATOR_LENGHT)
                {
                    return 0;
                }
                op_command = (wifi_operator_t)k;
            }
            switch (op_command)
            {
            case SET:
                xSemaphoreTake(mutex_light, portMAX_DELAY);
                light = token.toInt();
                xSemaphoreGive(mutex_light);
                break;
            case GET:
                xSemaphoreTake(mutex_light, portMAX_DELAY);
                local->printf("%d", light);
                xSemaphoreGive(mutex_light);
                break;
            default:
                return 0;
            }
            break;
#endif
        default:
            return 0;
        }
        break;
    case REFRESH:
        current_index = sub_msg.indexOf(STRING_DELIMITER);
        if (current_index <= 0)
        {
            current_index = sub_msg.length();
        }
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index + 1);
        while (token.compareTo(WIFI_OPERATOR_TYPE_STRING[j]) != 0)
        {
            j++;
            vTaskDelay(1 / portTICK_PERIOD_MS);
            if (j == WIFI_OPERATOR_LENGHT)
            {
                return 0;
            }
            op_command = (wifi_operator_t)j;
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
        if (current_index <= 0)
        {
            current_index = sub_msg.length();
        }
        token = sub_msg.substring(0, current_index);
        sub_msg = sub_msg.substring(current_index + 1);
        while (token.compareTo(WIFI_SYSTEM_TYPE_STRING[j]) != 0)
        {
            j++;
            vTaskDelay(1 / portTICK_PERIOD_MS);
            if (j == WIFI_SYSTEM_LENGHT)
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
            local->printf("1");
            break;
        case RESTART:
            //todo add restart
            ESP.restart();

            break;
        case STOP:
            //todo add stop
            break;
        case WIFI:
            current_index = sub_msg.indexOf(STRING_DELIMITER);
            if (current_index <= 0)
            {
                current_index = sub_msg.length();
            }
            token = sub_msg.substring(0, current_index);
            sub_msg = sub_msg.substring(current_index + 1);
            while (token.compareTo(WIFI_AUTH_TYPE_STRING[k]) != 0)
            {
                k++;
                vTaskDelay(1 / portTICK_PERIOD_MS);
                if (k == WIFI_AUTH_LENGHT)
                {
                    return 0;
                }
                auth_command = (wifi_auth_t)k;
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
            break;
        case GET_ID:
            local->printf("%d", DEVICE_ID);
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
