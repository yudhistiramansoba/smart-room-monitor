#include "Alarm.h"
#include "MQTT.h"

#define PIN_LED_G 25
#define PIN_LED_R 26
#define PIN_BUZZER 33
#define PIN_BTN 14

float BATAS_SUHU = 30.0;

bool alarmDiakui = false;
bool statusAlarmTerakhir = false;
bool buzzerAktif = false;

void mulaiAlarm() {
    pinMode(PIN_LED_G, OUTPUT);
    pinMode(PIN_LED_R, OUTPUT);
    pinMode(PIN_BUZZER, OUTPUT);
    pinMode(PIN_BTN, INPUT_PULLUP);
}

void aturAlarm(float suhu) {
    static bool tombolTerakhir = HIGH;
    static bool tombolStabil = HIGH;
    static unsigned long waktuDebounce = 0;

    bool tombol = digitalRead(PIN_BTN);

    if (tombol != tombolTerakhir) {
        waktuDebounce = millis();
    }

    if (millis() - waktuDebounce > 50) {
        if (tombol != tombolStabil) {
            tombolStabil = tombol;
            if (tombolStabil == LOW) {
                Serial.println("TOMBOL DITEKAN");
            }
            if (tombolStabil == LOW && suhu > BATAS_SUHU) {
                alarmDiakui = true;
                Serial.println("ALARM SUDAH MATI!");
            }
        }
    }

    tombolTerakhir = tombol;

    if (isnan(suhu)) return;

    if (suhu <= BATAS_SUHU) {
        alarmDiakui = false;
    }

bool suhuTinggi = suhu > BATAS_SUHU;
buzzerAktif = suhuTinggi && !alarmDiakui;

digitalWrite(PIN_LED_R, suhuTinggi);
digitalWrite(PIN_BUZZER, buzzerAktif);
digitalWrite(PIN_LED_G, !suhuTinggi);

if (buzzerAktif != statusAlarmTerakhir) {
    Serial.println(buzzerAktif ? "Buzzer: ON" : "Buzzer: OFF");
    kirimEventBuzzer(buzzerAktif);
    statusAlarmTerakhir = buzzerAktif;
}
}
