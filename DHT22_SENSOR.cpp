#include "DHT22_Sensor.h"

DHT22_Sensor::DHT22_Sensor(uint8_t pin) : dht(pin, DHT22) {}

void DHT22_Sensor::begin() {
  dht.begin();
}

float DHT22_Sensor::readTemperature() {
  float t = dht.readTemperature();
  return isnan(t) ? NAN : t;
}

float DHT22_Sensor::readHumidity() {
  float h = dht.readHumidity();
  return isnan(h) ? NAN : h;
}
