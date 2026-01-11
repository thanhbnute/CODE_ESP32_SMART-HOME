#ifndef KETNOI_H
#define KETNOI_H

#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

void KetNoi_WiFi();
void KhoiTao_MQTT();
void Gui_MQTT(const char* topic, const String &payload);

// Cho phép file .cpp khác truy cập mqttClient
extern PubSubClient mqttClient;

#endif
