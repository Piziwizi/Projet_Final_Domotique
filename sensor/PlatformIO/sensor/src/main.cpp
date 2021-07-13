
#include "DHT.h"
#include "wifi_com.h"

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

  init_wifi();
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
  else{
    Serial.print(F("Temperature: "));
    Serial.print(temperature);
    Serial.println(F("°C "));
  }
  vTaskDelay(20000 / portTICK_PERIOD_MS);

  return temperature;
}

