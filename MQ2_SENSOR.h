#ifndef _MQ2_SENSOR_
#define _MQ2_SENSOR_

#include <Arduino.h>
#include <MQUnifiedsensor.h>

class MQ2_Sensor {
public:
    // analogPin: GPIO34
    // adcVoltage: 3.3V cho ESP32
    MQ2_Sensor(uint8_t analogPin, float adcVoltage = 3.3);

    void begin();              // init ADC + MQ2
    bool calibrate();          // hiệu chuẩn, KHÔNG làm treo ESP
    bool isAvailable();        // sensor còn hoạt động hay không

    float readSmoke();         // ppm smoke
    float readLPG();           // ppm LPG
    float readCO();            // ppm CO

private:
    MQUnifiedsensor mq2;
    uint8_t pin;
    float adcVoltage;
    bool available;
};

#endif
