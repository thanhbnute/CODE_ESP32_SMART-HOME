#ifndef THIETbI_H
#define THIETbI_H
#include <Arduino.h>


struct Device {
  const char* name;
  const char* topic;
  uint8_t pin;
  bool state;
};

#define DEVICE_COUNT 8        // 🟢 Số lượng thiết bị cố định

extern Device devices[];

#endif
