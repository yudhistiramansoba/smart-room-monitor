#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "MQTT.h"
#include "Alarm.h"

// Wi-Fi Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// ID perangkat
const char* deviceID = "sr_yudhi_20261107";
const char* clientID = "MQTTExplorer_Yudhi";
// Topik MQTT
String topicTelemetry = "smartroom/" + String(deviceID) + "/telemetry";
String topicCommand = "smartroom/" + String(deviceID) + "/command";
String topicStatus = "smartroom/" + String(deviceID) + "/status";
String topicEvent = "smartroom/" + String(deviceID) + "/event";

WiFiClient espClient;
PubSubClient client(espClient);

unsigned long waktuReconnect = 0;

void callback(char* topic, byte* payload, unsigned int length) {
    JsonDocument doc;

    DeserializationError error = deserializeJson(doc, payload, length);

    if (error) {
        Serial.println("Gagal membaca perintah MQTT");
        return;
    }

    if (doc["threshold"].is<float>() || doc["threshold"].is<int>()) {
        BATAS_SUHU = doc["threshold"].as<float>();

        Serial.print("Batas suhu diubah menjadi: ");
        Serial.println(BATAS_SUHU);
    }
}

// Menghubungkan Wi-Fi dan MQTT
void mulaiMQTT() {
    WiFi.begin(ssid, password);

    client.setServer(mqttServer, mqttPort);
    client.setCallback(callback);

    client.setKeepAlive(30);
}

// Memastikan koneksi tetap aktif
void prosesMQTT() {
    if (WiFi.status() != WL_CONNECTED) {
        if (millis() - waktuReconnect >= 5000) {
            waktuReconnect = millis();

            Serial.println("Menghubungkan ulang Wi-Fi...");
            WiFi.begin(ssid, password);
        }
        return;
    }

    if (!client.connected()) {
        if (millis() - waktuReconnect >= 5000) {
            waktuReconnect = millis();

            Serial.println("Menghubungkan ke MQTT...");

            String clientID = "ESP32-" + String(deviceID);

            if (client.connect(
                clientID.c_str(),
                topicStatus.c_str(),
                0,
                true,
                "offline"
            )) {
                Serial.println("MQTT terhubung!");

                client.publish(topicStatus.c_str(), "online", true);
                client.subscribe(topicCommand.c_str());
            }
        }
    }

    client.loop();
}

// Mengirim data sensor dalam format JSON
void kirimTelemetry(float suhu, float kelembapan, int cahaya, int gerakan, bool buzzer) {
    if (!client.connected()) {
        Serial.println("MQTT belum terhubung!");
        return;
    }

    JsonDocument doc;

    doc["temperature"] = suhu;
    doc["humidity"] = kelembapan;
    doc["light"] = cahaya;
    doc["motion"] = gerakan;
    doc["buzzer"] = buzzer;

    char payload[256];

    serializeJson(doc, payload);
    bool berhasil = client.publish(topicTelemetry.c_str(), payload);

}
void kirimEventBuzzer(bool buzzer) {
    if (!client.connected()) {
        Serial.println("MQTT belum terhubung!");
        return;
    }

    JsonDocument doc;

    doc["event"] = "buzzer_changed";
    doc["buzzer"] = buzzer;

    char payload[128];

    serializeJson(doc, payload);

    bool berhasil = client.publish(
        topicEvent.c_str(),
        payload
    );

}
bool statusMQTT() {
    return client.connected();
}