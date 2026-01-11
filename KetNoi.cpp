#include "KetNoi.h"
#include "ThietBi.h"

const char* ssid = "THANHLAPTOP";
const char* password = "12345611";

const char* mqtt_server   = "108cfcab1b884677af95757a5eba63fd.s1.eu.hivemq.cloud";
const int   mqtt_port     = 8883;
const char* mqtt_username = "SMARTHOME";
const char* mqtt_password = "123456Aa";

WiFiClientSecure espClient;
PubSubClient mqttClient(espClient);

// ================= WIFI =================
void KetNoi_WiFi() {
  WiFi.begin(ssid, password);

  Serial.print("Đang kết nối WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n✔ WiFi OK");
}


// ============== MQTT CALLBACK (NHẬN LỆNH TỪ WEB/APP) ==============
void callback(char* topic, byte* payload, unsigned int length) {
    String value = "";
    for (int i = 0; i < length; i++)
        value += (char)payload[i];

    Serial.printf("MQTT nhận: %s -> %s\n", topic, value.c_str());

    for (int i = 0; i < DEVICE_COUNT; i++) {
        if (String(topic) == devices[i].topic) {
            bool state = (value == "true");

            devices[i].state = state;
            digitalWrite(devices[i].pin, state ? HIGH : LOW);

            Serial.printf("Thiết bị %s: %s\n",
                          devices[i].name,
                          state ? "ON" : "OFF");
        }
    }
}


// ================ MQTT TLS ================
void KhoiTao_MQTT() {
  mqttClient.setCallback(callback);
  mqttClient.setServer(mqtt_server, mqtt_port);
  espClient.setInsecure();   

  mqttClient.setKeepAlive(60);
  mqttClient.setSocketTimeout(60);

  Serial.println("Kết nối MQTT...");

  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client", mqtt_username, mqtt_password)) {

      Serial.println("✔ MQTT OK!");

      // Subscribe toàn bộ topic thiết bị
      for (int i = 0; i < DEVICE_COUNT; i++) {
        mqttClient.subscribe(devices[i].topic);
        Serial.print("Subscribed: ");
        Serial.println(devices[i].topic);
      }

    } else {
      Serial.print("❌ MQTT lỗi: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}


// ================ GỬI MQTT (Gửi cảm biến) =================
void Gui_MQTT(const char* topic, const String &payload) {
  mqttClient.publish(topic, payload.c_str());

  Serial.print("📤 MQTT → ");
  Serial.print(topic);
  Serial.print(" : ");
  Serial.println(payload);
}
