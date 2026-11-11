#include "Display.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define LEBAR 128
#define TINGGI 64
#define ALAMAT_OLED 0x3C

Adafruit_SSD1306 oled(LEBAR, TINGGI, &Wire, -1);

void mulaiDisplay() {
    Wire.begin();

    if (!oled.begin(SSD1306_SWITCHCAPVCC, ALAMAT_OLED)) {
        Serial.println("OLED gagal diinisialisasi!");
        return;
    }

    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.setTextSize(1);
    oled.display();
}

void tampilkanDisplay(float suhu, float kelembapan, int cahaya, bool gerakan) {
    oled.clearDisplay();
    oled.setCursor(0, 0);

    oled.println("SMART ROOM MONITOR");

    oled.print("Suhu: ");
    if (isnan(suhu)) oled.println("Error");
    else oled.println(suhu);

    oled.print("Kelembapan: ");
    if (isnan(kelembapan)) oled.println("Error");
    else oled.println(kelembapan);

    oled.print("Cahaya: ");
    oled.println(cahaya);

    oled.print("Gerakan: ");
    oled.println(gerakan ? "Terdeteksi" : "Tidak");

    oled.display();
}