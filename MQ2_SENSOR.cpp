#include "MQ2_Sensor.h"

#define BOARD "ESP32"
#define TYPE  "MQ-2"

MQ2_Sensor::MQ2_Sensor(uint8_t analogPin, float voltage)
    : mq2(BOARD, voltage, 12, analogPin, TYPE),
      pin(analogPin),
      adcVoltage(voltage),
      available(false) {
}

void MQ2_Sensor::begin() {
    mq2.init();
    mq2.setRegressionMethod(1);
    mq2.setA(605.18);
    mq2.setB(-3.937);
    mq2.setRL(10.0); // Rload

    Serial.println("[MQ2] Calibrating in clean air...");
    float r0 = 0;
    for (int i = 0; i < 10; i++) {
        mq2.update();
        r0 += mq2.calibrate(9.83);  // 9.83 là tỉ lệ Rs/R0 cho không khí sạch
        delay(1000);
    }
    r0 /= 10.0;

    if (r0 > 0 && r0 < 10000) {
        mq2.setR0(r0);
        available = true;
        Serial.printf("[MQ2] Calibration OK - R0 = %.2f\n", r0);
    } else {
        available = false;
        Serial.println("[MQ2] Calibration failed.");
    }
}


bool MQ2_Sensor::calibrate() {
    Serial.println("⏳ MQ-2 warming up & calibrating...");
    delay(30000); // MQ-2 cần nóng ~30s (rất quan trọng)

    float r0 = 0;
    int validCount = 0;

    for (int i = 0; i < 10; i++) {
        mq2.update();
        float val = mq2.calibrate(9.83); // Clean air ratio

        if (val > 0 && !isinf(val)) {
            r0 += val;
            validCount++;
        }

        delay(1000);
    }

    if (validCount < 5) {
        Serial.println("❌ MQ-2 calibration FAILED");
        Serial.println("👉 Kiểm tra:");
        Serial.println("- AO có CHIA ÁP xuống 3.3V chưa");
        Serial.println("- VCC phải là 5V");
        Serial.println("- MQ-2 có được làm nóng đủ 30s");
        Serial.println("- Có thể MQ-2 bị rút ra");

        available = false;
        return false;   // ❗ KHÔNG DỪNG ESP32
    }

    r0 /= validCount;
    mq2.setR0(r0);

    Serial.print("✅ MQ-2 OK | R0 = ");
    Serial.println(r0);

    available = true;
    return true;
}

bool MQ2_Sensor::isAvailable() {
    return available;
}

float MQ2_Sensor::readSmoke() {
    if (!available) return -1;
    mq2.update();
    mq2.setA(574.25);
    mq2.setB(-2.222);
    return mq2.readSensor();
}

float MQ2_Sensor::readLPG() {
    if (!available) return -1;
    mq2.update();
    mq2.setA(574.25);
    mq2.setB(-2.222);
    return mq2.readSensor();
}

float MQ2_Sensor::readCO() {
    if (!available) return -1;
    mq2.update();
    mq2.setA(36974);
    mq2.setB(-3.109);
    return mq2.readSensor();
}
