#include "Sensor.h"
#include <DHT.h>

#define PIN_DHT 4
#define PIN_LDR 34
#define PIN_PIR 27

DHT dht(PIN_DHT, DHT22);

float suhu = 0;
float kelembapan = 0;
int cahaya = 0;
bool gerakan = false;

void mulaiSensor() {
    dht.begin();
}

void bacaSensor() {
    suhu = dht.readTemperature();
    kelembapan = dht.readHumidity();
    cahaya = analogRead(PIN_LDR);
    gerakan = digitalRead(PIN_PIR);
}