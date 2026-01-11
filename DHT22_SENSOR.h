#ifndef _DHT22_SENSOR_
#define _DHT22_SENSOR_

#include <DHT.h>

class DHT22_Sensor {
public:
  DHT22_Sensor(uint8_t pin);
  void begin();
  float readTemperature();
  float readHumidity();

private:
  DHT dht;
};

#endif
