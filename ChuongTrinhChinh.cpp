#include "ChuongTrinhChinh.h"
#include "DHT22_SENSOR.h"
#include "MQ2_SENSOR.h"
#include "BH1750_Sensor.h"
#include "KetNoi.h"
#include <Wire.h>
#include "ThietBi.h"

// ============================
//       CẢM BIẾN TỪNG PHÒNG
// ============================

DHT22_Sensor dhtKitchen(25);
DHT22_Sensor dhtLiving(26);
DHT22_Sensor dhtBed(27);

MQ2_Sensor gasKitchen(34);

// BH1750 — 2 phòng
BH1750_Sensor lightLiving(0x23); // ADDR GND
BH1750_Sensor lightBed(0x5C);    // ADDR VCC

// ============================
//       BIẾN GIÁ TRỊ
// ============================

float k_temp = 0, k_humi = 0, k_gas = 0;
float l_temp = 0, l_humi = 0, l_light = 0;
float b_temp = 0, b_humi = 0, b_light = 0;

// ============================
//        HÀM KHỞI TẠO
// ============================

void KhoiTao()
{
    Serial.begin(115200);
    Serial.println("\n===== SMART HOME ESP32 STARTING =====");
    Wire.begin(21, 22); // I2C
    dhtKitchen.begin();
    dhtLiving.begin();
    dhtBed.begin();
    gasKitchen.begin();

    lightLiving.begin();
    lightLiving.setInterval(1000);

    lightBed.begin();
    lightBed.setInterval(1000);

    for (int i = 0; i < DEVICE_COUNT; i++) {
        pinMode(devices[i].pin, OUTPUT);
        digitalWrite(devices[i].pin, LOW);
    }

     KetNoi_WiFi();
     KhoiTao_MQTT();
}

// ============================
//     ĐỌC DỮ LIỆU CẢM BIẾN
// ============================

void DocCamBien()
{
    // ---------- Kitchen ----------
    k_temp = dhtKitchen.readTemperature();
    k_humi = dhtKitchen.readHumidity();
    k_gas = gasKitchen.readCO();

    // ---------- Livingroom ----------
    l_temp = dhtLiving.readTemperature();
    l_humi = dhtLiving.readHumidity();
    lightLiving.update();
    l_light = lightLiving.getLux();

    // ---------- Bedroom ----------
    b_temp = dhtBed.readTemperature();
    b_humi = dhtBed.readHumidity();
    lightBed.update();
    b_light = lightBed.getLux();

    // Debug log
    Serial.println("\n----- SENSOR VALUES -----");
    Serial.printf("[Kitchen]    Temp: %.1f  Humi: %.1f  Gas: %.1f\n", k_temp, k_humi, k_gas);
    Serial.printf("[Livingroom] Temp: %.1f  Humi: %.1f  Light: %.1f\n", l_temp, l_humi, l_light);
    Serial.printf("[Bedroom]    Temp: %.1f  Humi: %.1f  Light: %.1f\n", b_temp, b_humi, b_light);
}

// ============================
//       GỬI MQTT TỪNG PHÒNG
// ============================

void GuiPhong_Kitchen()
{
    if (isnan(k_temp)) k_temp = 0;
    if (isnan(k_humi)) k_humi = 0;
    if (isnan(k_gas)) k_gas = 0;

    String json = "{";
    json += "\"temp\":" + String(k_temp) + ",";
    json += "\"humidity\":" + String(k_humi) + ",";
    json += "\"gas\":" + String(k_gas);
    json += "}";

    Gui_MQTT("rooms/kitchen/sensors", json);
}

void GuiPhong_Livingroom()
{
    if (isnan(l_temp)) l_temp = 0;
    if (isnan(l_humi)) l_humi = 0;
    if (isnan(l_light)) l_light = 0;

    String json = "{";
    json += "\"temp\":" + String(l_temp) + ",";
    json += "\"humidity\":" + String(l_humi) + ",";
    json += "\"light\":" + String(l_light);
    json += "}";

    Gui_MQTT("rooms/livingroom/sensors", json);
}

void GuiPhong_Bedroom()
{
    if (isnan(b_temp)) b_temp = 0;
    if (isnan(b_humi)) b_humi = 0;
    if (isnan(b_light)) b_light = 0;

    String json = "{";
    json += "\"temp\":" + String(b_temp) + ",";
    json += "\"humidity\":" + String(b_humi) + ",";
    json += "\"light\":" + String(b_light);
    json += "}";

    Gui_MQTT("rooms/bedroom/sensors", json);
}

// ============================
//       GỬI TẤT CẢ PHÒNG
// ============================

void GuiTatCaPhong()
{
    GuiPhong_Kitchen();
    delay(100);
    GuiPhong_Livingroom();
    delay(100);
    GuiPhong_Bedroom();
    delay(100);
}

// ============================
//       LOOP CHÍNH
// ============================

void ChuongTrinh()
{
     if (!mqttClient.connected()) {
        KhoiTao_MQTT();
     }

     mqttClient.loop();

    static unsigned long last = 0;
    if (millis() - last > 5000) { 
        last = millis();
        DocCamBien();
        GuiTatCaPhong();
    }
}
