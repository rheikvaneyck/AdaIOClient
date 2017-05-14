
/* 
 *  Ein Adafruit IO Client basierend auf dne Adafruit IO Beispielen:
 *  Adafruit IO Publish Example
 *  Adafruit invests time and resources providing this open source code.
 *  Please support Adafruit and open source hardware by purchasing
 *  products from Adafruit!
 *  Written by Todd Treece for Adafruit Industries
 *  Licensed under the MIT license.
 *  
 *  Edited and extended by Marcus Nasarek, 2017
 */

 /*
  * Abhängigkeiten von den folgenden Bibliotheken:
  * - DHT
  * - ArduinoHttpClient
  * - Adafruit Unified Sensor
  * - Adafruit MGTT Library
  * - Adafruit IO Arduino
  */
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "config.h" // Wifi-Zugang und Adafruit IO Key anpassen
#include <user_interface.h>

#define DHTPIN 2      // Sensor an GPIO #2 
#define DHTTYPE DHT22 //Sensor-Typ
#define LED 13        // LED an GPIO #13
#define INTERVAL 60   // Konstante für Update-Intervall in s

AdafruitIO_Feed *temperatur = io.feed("Temperatur");
AdafruitIO_Feed *humidity = io.feed("Luftfeuchte");
AdafruitIO_Feed *led = io.feed("LED");
DHT_Unified dht(DHTPIN, DHTTYPE);

os_timer_t myTimer;
uint32_t delayMS;
float temp = 0.0;
int hum = 0;
boolean update_io = true;
boolean tick = false;
int counter = 0;

void setup() {
  Serial.begin(115200);
  while(! Serial);
  Serial.print("Verbindung mit Adafruit IO");
  io.connect();
  led->onMessage(handleMsg);
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
  pinMode(LED, OUTPUT);
  os_timer_setfn(&myTimer, timerCallback, NULL);
  os_timer_arm(&myTimer, 1000, true);
}

void loop() {
  delay(500);
  io.run();
  if(tick) {
    if(counter>INTERVAL) {
      update_io = true;
      counter = 0;
    } else {
      counter++;
    }
    tick = false;
  }
  if(update_io) {
    sensors_event_t event;  
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
      Serial.println("Kann Temperatur nicht lesen!");
    }
    else {
      temp = event.temperature;
      Serial.print("Temperatur: ");
      Serial.print(temp);
      Serial.println(" *C");
      temperatur->save(temp);
    }
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
      Serial.println("Kann Luftfeuchte nicht lesen!");
    }
    else {
      hum = event.relative_humidity;
      Serial.print("Luftfeuchte: ");
      Serial.print(hum);
      Serial.println("%");
      humidity->save(hum);
    }
  
    Serial.print("Sende Temperatur -> ");
    Serial.println(temp);
    Serial.print("Sende Luftfeuchte -> ");
    Serial.println(hum);
    update_io = false;
  }
}
void handleMsg(AdafruitIO_Data *data) {
  if(String(data->value()) == "ON") {
    Serial.println("Schalte LED an");
    digitalWrite(LED, HIGH);
  } else {
    Serial.println("Schalte LED aus");
    digitalWrite(LED, LOW);
  }
}

void timerCallback(void *pArg) {
  tick = true;
}

