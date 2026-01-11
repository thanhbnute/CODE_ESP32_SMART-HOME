#ifndef _BH1750_SENSOR_H
#define _BH1750_SENSOR_H

#include <Arduino.h>
#include <Wire.h>
#include <BH1750.h>

class BH1750_Sensor {
public:
    BH1750_Sensor(uint8_t address = 0x23, TwoWire *wire = &Wire);

    void begin();                          // Gọi 1 lần sau Wire.begin
    void update();                         // Gọi trong loop hoặc theo chu kỳ
    float getLux();                        // Lấy giá trị lux cuối cùng

    bool isConnected();                    // Kiểm tra thiết bị có phản hồi không
    void setInterval(unsigned long ms);    // Đặt chu kỳ đọc

private:
    BH1750 lightMeter;
    TwoWire* wire;
    uint8_t _address;

    float luxValue = 0;
    unsigned long lastRead = 0;
    unsigned long readInterval = 1000; // mặc định 1 giây
};

#endif
