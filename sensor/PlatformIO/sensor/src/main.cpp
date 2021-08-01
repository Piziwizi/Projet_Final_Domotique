#include "define.h"
#include "DHT.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <WiFi.h>
#include <WiFiMulti.h>

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;
WiFiServer server_device(3334);

SemaphoreHandle_t mutex_read_temp, mutex_read_light;
float read_temp, read_light = 0;

SemaphoreHandle_t mutex_heat, mutex_light;
bool heat, light = 0;

SemaphoreHandle_t mutex_button_heat, mutex_button_light;
uint32_t light_switch = 0;
int32_t temp_switch = 0;

double old_temp = NAN;
uint32_t fail = 0;

//************  function declaration  ************
float read_temperature(float *Temperature);
void wifi_client(void *parameter);
void init_wifi(void);
void apply_gpio(void *parameter);
void wifi_client(void *parameter);
void wifi_server(void *parameter);
uint32_t decode_message(String msg, WiFiClient *local);

void setup()
{

    Serial.begin(115200);
    dht.begin();
    pinMode(BUTTON_LIGHT, INPUT);
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);

    pinMode(RELAY_HEAT, OUTPUT);
    pinMode(RELAY_FAN, OUTPUT);
    pinMode(RELAY_LIGHT, OUTPUT);

    mutex_read_temp = xSemaphoreCreateMutex();
    mutex_read_light = xSemaphoreCreateMutex();
    mutex_heat = xSemaphoreCreateMutex();
    mutex_light = xSemaphoreCreateMutex();
    mutex_button_heat = xSemaphoreCreateMutex();
    mutex_button_light = xSemaphoreCreateMutex();

    if (mutex_read_temp == NULL ||
        mutex_read_light == NULL ||
        mutex_heat == NULL ||
        mutex_light == NULL ||
        mutex_button_heat == NULL ||
        mutex_button_light == NULL)
    {
        ESP.restart();
    }
    digitalWrite(RELAY_HEAT, RELAY_OFF);
    digitalWrite(RELAY_FAN, RELAY_OFF);
    digitalWrite(RELAY_LIGHT, RELAY_OFF);

    xTaskCreate(
        apply_gpio,   // Function that should be called
        "Apply GPIO", // Name of the task (for debugging)
        10000,        // Stack size (bytes)
        NULL,         // Parameter to pass
        1,            // Task priority
        NULL          // Task handle
    );

    init_wifi();
}

void loop()
{
    read_temperature(&read_temp);
}
//*************   FUNCTION   ***************//
float read_temperature(float *Temperature) //
{
    *Temperature = dht.readTemperature(); // put value in the global variable
    float temperature = *Temperature;     // to stop accessing the global variable for no reason

    if (isnan(temperature)) // Check if any reads failed and exit early (to try again)
    {
        Serial.println(F("Failed to read from DHT sensor!"));
        fail++;
    }
    else
    {
        xSemaphoreTake(mutex_read_temp, portMAX_DELAY);
        old_temp = temperature;
        xSemaphoreGive(mutex_read_temp);
        Serial.print(F("Temperature: "));
        Serial.print(temperature);
        Serial.println(F("°C "));
    }

    if (fail == 50)
    { //todo check why it fails when the resistance is running
        fail = 0;
        xSemaphoreTake(mutex_read_temp, portMAX_DELAY);
        old_temp = temperature;
        xSemaphoreGive(mutex_read_temp);
    }

    vTaskDelay(500 / portTICK_PERIOD_MS);

    return temperature;
}

void apply_gpio(void *parameter)
{
    uint32_t previous_light = 0;
    uint32_t previous_temp1 = 0;
    uint32_t previous_temp2 = 0;

    for (;;)
    {
        //Relays ---------------------------------
        xSemaphoreTake(mutex_heat, portMAX_DELAY);
        digitalWrite(RELAY_HEAT, heat ? RELAY_ON : RELAY_OFF);
        digitalWrite(RELAY_FAN, !heat ? RELAY_ON : RELAY_OFF);
        xSemaphoreGive(mutex_heat);

        xSemaphoreTake(mutex_light, portMAX_DELAY);
        digitalWrite(RELAY_LIGHT, light ? RELAY_ON : RELAY_OFF);
        xSemaphoreGive(mutex_light);


        //Buttons ---------------------------------
        vTaskDelay(50 / portTICK_PERIOD_MS); //50 ms for debounce
        if (previous_temp1 != digitalRead(BUTTON_UP))
        {
            previous_temp1 = digitalRead(BUTTON_UP);

            if (previous_temp1 == 0)
            {
                xSemaphoreTake(mutex_button_heat, portMAX_DELAY);
                temp_switch++;
                xSemaphoreGive(mutex_button_heat);
            }
            Serial.println(temp_switch);
        }

        if (previous_temp2 != digitalRead(BUTTON_DOWN))
        {
            previous_temp2 = digitalRead(BUTTON_DOWN);

            if (previous_temp2 == 0)
            {
                xSemaphoreTake(mutex_button_heat, portMAX_DELAY);
                temp_switch--;
                xSemaphoreGive(mutex_button_heat);
            }
            Serial.println(temp_switch);
        }

        if (previous_light != digitalRead(BUTTON_LIGHT))
        {
            previous_light = digitalRead(BUTTON_LIGHT);
            if (previous_light == 0)
            {
                xSemaphoreTake(mutex_button_light, portMAX_DELAY);
                light_switch = 1;
                xSemaphoreGive(mutex_button_light);
            }
            Serial.println(light_switch);
        }
    }
}

void init_wifi(void)
{
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
    const char *host = "10.10.10.10"; // ip

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
            decode_message(line, &client);
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
                    decode_message(line, &pi);
                }
                vTaskDelay(1 / portTICK_PERIOD_MS);
            }
            pi.stop();
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

uint32_t decode_message(String msg, WiFiClient *local)
{
    String sub_msg = msg;

    int current_index = sub_msg.indexOf(STRING_DELIMITER);
    String token = sub_msg.substring(0, current_index);
    sub_msg = sub_msg.substring(current_index + 1);

    wifi_main_t main_command = UNKNOWN_MAIN;
    wifi_type_t type_command = UNKNOWN_TYPE;
    wifi_operator_t op_command = UNKNOWN_OPERATOR;
    wifi_system_t system_command = UNKNOWN_SYSTEM;
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
                xSemaphoreTake(mutex_read_temp, portMAX_DELAY);
                local->printf("%f\r", old_temp);  // Set the temp value
                xSemaphoreGive(mutex_read_temp);
                break;
            default:
                return 0;
            }
            break;
        case LIGHT:
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
                heat = (bool)sub_msg.toInt(); // Set the temp relay value
                xSemaphoreGive(mutex_heat);
                break;
            case GET:
                xSemaphoreTake(mutex_heat, portMAX_DELAY);
                local->printf("%d\r", heat); // Get the temp relay value
                xSemaphoreGive(mutex_heat);
                break;
            default:
                return 0;
            }
            break;
        case LIGHT:
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
                light = (bool)sub_msg.toInt(); // Set the light relay value
                xSemaphoreGive(mutex_light);
                break;
            case GET:
                xSemaphoreTake(mutex_light, portMAX_DELAY);
                local->printf("%d\r", light); // Get the light relay value
                xSemaphoreGive(mutex_light);
                break;
            default:
                return 0;
            }
            break;
        default:
            return 0;
        }
        break;
    case BUTTON:
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
                xSemaphoreTake(mutex_button_heat, portMAX_DELAY);
                local->printf("%d\r", temp_switch);  // send the value of the temp buttons
                temp_switch = 0;
                xSemaphoreGive(mutex_button_heat);
                break;
            default:
                return 0;
            }
            break;
        case LIGHT:
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
                xSemaphoreTake(mutex_button_light, portMAX_DELAY);
                local->printf("%d\r", light_switch); // send the value of the light button
                light_switch = 0;
                xSemaphoreGive(mutex_button_light);
                break;
            default:
                return 0;
            }
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
            local->printf("1"); //send that the sensor is started
            break;
        case RESTART:
            ESP.restart(); //restart the sensor
            break;
        case STOP:
            //todo add stop
            break;
        case STATUS_DEVICE:
            //todo add status
            break;
        case GET_ID:
            local->printf("%d\r", (uint32_t)DEVICE_ID);
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
