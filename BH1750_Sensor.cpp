#include "BH1750_Sensor.h"

BH1750_Sensor::BH1750_Sensor(uint8_t address, TwoWire *wire)
    : wire(wire), lightMeter(address), _address(address) {}

void BH1750_Sensor::begin() {
    if (isConnected()) {
        lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, _address, wire);
        delay(200);  // Cho cảm biến thời gian khởi động
        Serial.printf("✅ BH1750 (0x%02X) đã sẵn sàng.\n", _address);
    } else {
        Serial.printf("❌ BH1750 (0x%02X) KHÔNG kết nối được!\n", _address);
    }
}

bool BH1750_Sensor::isConnected() {
    wire->beginTransmission(_address);
    return (wire->endTransmission() == 0);
}

void BH1750_Sensor::setInterval(unsigned long ms) {
    readInterval = ms;
}

void BH1750_Sensor::update() {
    if (!isConnected()) {
        Serial.printf("⚠️  BH1750 (0x%02X) mất kết nối!\n", _address);
        return;
    }

    unsigned long now = millis();
    if (now - lastRead >= readInterval) {
        lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE, _address, wire);
        delay(180); // BH1750 cần ~120ms để đo xong
        float lux = lightMeter.readLightLevel();
        if (!isnan(lux) && lux > 0) {
            luxValue = lux;
            Serial.printf("[BH1750 0x%02X] 🌞 Lux = %.2f\n", _address, lux);
        } else {
            Serial.printf("[BH1750 0x%02X] ⚠️ Giá trị lux không hợp lệ\n", _address);
        }
        lastRead = now;
    }
}

float BH1750_Sensor::getLux() {
    return luxValue;
}
