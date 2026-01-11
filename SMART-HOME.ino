#include "ChuongTrinhChinh.h"
#include <Wire.h>
void setup() {
  Serial.begin(115200);
  KhoiTao();        // Hàm khởi tạo toàn hệ thống
}

void loop() {
  ChuongTrinh();    // Hàm vòng lặp chính (WiFi, MQTT, sensor)
}
