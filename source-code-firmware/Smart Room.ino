
#include "Sensor.h"
#include "Alarm.h"
#include "Display.h"
#include "MQTT.h"

unsigned long waktuBaca = 0;
const unsigned long INTERVAL = 5000;

bool statusBuzzerSebelumnya = false;

void setup() {
  Serial.begin(115200);

  mulaiSensor();
  mulaiAlarm();
  mulaiDisplay();
  mulaiMQTT();

  Serial.println("Smart Room Monitor dimulai!");
}


void loop() {
  prosesMQTT();

  if (millis() - waktuBaca >= INTERVAL) {
    waktuBaca = millis();

    bacaSensor();
    aturAlarm(suhu);
    tampilkanDisplay(suhu, kelembapan, cahaya, gerakan);

    kirimTelemetry(
      suhu,
      kelembapan,
      cahaya,
      gerakan,
      buzzerAktif
    );

    Serial.print("MQTT: ");
    Serial.println(statusMQTT() ? "CONNECTED" : "DISCONNECTED");

    Serial.print("Suhu: ");
    Serial.println(suhu);

    Serial.print("Kelembapan: ");
    Serial.println(kelembapan);

    Serial.print("Cahaya: ");
    Serial.println(cahaya);

    Serial.print("Gerakan: ");
    Serial.println(gerakan ? "Ada" : "Tidak Ada");

    Serial.print("Buzzer: ");
    Serial.println(buzzerAktif ? "ON" : "OFF");

    Serial.println("-----------------------------------");
  }

}
