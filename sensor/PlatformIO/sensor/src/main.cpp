
#include "DHT.h"
#include <WiFi.h>
#include <WiFiMulti.h>

#define DHTPIN 4
#define relay_heat 23
#define relay_light 22
#define bouton 19

#define DHTTYPE DHT11 // DHT 11

DHT dht(DHTPIN, DHTTYPE);

//************  déclaration variable   ************
float read_temp = 0;
bool heat, light = 0;

//************  déclaration fonction   ************
float read_temperature(float *Temperature);
void wifi_client(void *parameter);

void setup()
{

  pinMode(relay_heat, OUTPUT);
  pinMode(relay_light, OUTPUT);
  pinMode(bouton, INPUT); // pour tester

  Serial.begin(115200);
  dht.begin();

  xTaskCreate(
      wifi_client,   // Function that should be called
      "Wifi Client", // Name of the task (for debugging)
      5000,          // Stack size (bytes)
      NULL,          // Parameter to pass
      1,             // Task priority
      NULL           // Task handle
  );
}

void loop()
{

  read_temperature(&read_temp);

  heat = digitalRead(bouton); // pour tester

  if (heat == HIGH)
  {
    digitalWrite(relay_heat, HIGH);
  }
  else
  {
    digitalWrite(relay_heat, LOW);
  }

  if (light == HIGH)
  {
    digitalWrite(relay_light, HIGH);
  }
  else
  {
    digitalWrite(relay_light, LOW);
  }
}
//*************   FONCTION   ***************//

float read_temperature(float *Temperature) //
{
  *Temperature = dht.readTemperature(); // put value in the global variable
  float temperature = *Temperature;     // to stop accessing the global variable for no reason

  if (isnan(temperature)) // Check if any reads failed and exit early (to try again)
  {
    Serial.println(F("Failed to read from DHT sensor!"));
  }

  Serial.print(F("Temperature: "));
  Serial.print(temperature);
  Serial.println(F("°C "));
  vTaskDelay(2000 / portTICK_PERIOD_MS);

  return temperature;
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
    client.print("GET /index.html HTTP/1.1\n\n");

    int maxloops = 0;

    //wait for the server's reply to become available
    while (!client.available() && maxloops < 1000)
    {
      maxloops++;
      vTaskDelay(1 / portTICK_PERIOD_MS);
    }
    if (client.available() > 0)
    {
      //read back one line from the server
      String line = client.readStringUntil('\r');
      Serial.println(line);
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
