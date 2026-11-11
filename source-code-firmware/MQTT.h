#ifndef MQTT_H
#define MQTT_H

void mulaiMQTT();
void prosesMQTT();
void kirimTelemetry(float suhu, float kelembapan, int cahaya, int gerakan, bool buzzer);
void kirimEventBuzzer(bool buzzer);
bool statusMQTT();

#endif