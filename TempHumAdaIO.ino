#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "config.h"
#define DHTPIN 2 
#define DHTTYPE DHT22
AdafruitIO_Feed *temperatur = io.feed("Temperatur");
AdafruitIO_Feed *humidity = io.feed("Humidity");
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;
float temp = 0.0;
int hum = 0;
void setup() {
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Connecting to Adafruit IO");
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println(io.statusText());
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  io.run();
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" *C");
    temperatur->save(temp);
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
    humidity->save(hum);
  }

  Serial.print("sending temperatur -> ");
  Serial.println(temp);
  Serial.print("sending humidity -> ");
  Serial.println(hum);
  delay(10000);
}
