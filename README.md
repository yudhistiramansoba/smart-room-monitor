# Smart Room Monitor

Sistem pemantauan kondisi ruangan berbasis ESP32 yang menggunakan sensor untuk membaca suhu, kelembapan, intensitas cahaya, dan pergerakan. Data ditampilkan melalui OLED dan dashboard serta dikirim menggunakan protokol MQTT.

## 1. Komponen

- ESP32
- DHT22
- LDR Sensor Module
- PIR HC-SR501
- OLED SSD1306 0.96"
- LED Hijau
- LED Merah
- Buzzer
- Push Button
- Resistor

## 2. Konfigurasi Pin

| Komponen | Pin ESP32 |
|---|---|
| DHT22 Data | GPIO 4 |
| LDR Analog | GPIO 34 |
| PIR OUT | GPIO 27 |
| LED Hijau | GPIO 25 |
| LED Merah | GPIO 26 |
| Buzzer (+) | GPIO 33 |
| Push Button | GPIO 14 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |

Alamat I2C OLED: `0x3C`.

## 3. Fitur Sistem

- Monitoring suhu dan kelembapan.
- Monitoring intensitas cahaya.
- Deteksi pergerakan.
- Tampilan data melalui OLED.
- Alarm suhu menggunakan LED dan buzzer.
- Tombol acknowledge untuk alarm.
- Pengiriman data sensor melalui MQTT setiap 5 detik.
- Status perangkat online/offline menggunakan MQTT LWT.
- Pengaturan threshold suhu melalui MQTT.
- Monitoring melalui dashboard berbasis web.

## 4. Konfigurasi MQTT

**Broker:** `broker.hivemq.com`

**Port ESP32:** `1883`

**WebSocket Dashboard:** `ws://broker.hivemq.com:8000/mqtt`

**Device ID:** `sr_yudhi_20261107`

| Topic | Fungsi |
|---|---|
| `smartroom/sr_yudhi_20261107/telemetry` | Pengiriman data sensor |
| `smartroom/sr_yudhi_20261107/command` | Menerima perintah perubahan threshold |
| `smartroom/sr_yudhi_20261107/status` | Status online/offline |
| `smartroom/sr_yudhi_20261107/event` | Informasi perubahan status buzzer |

Contoh perintah perubahan threshold:

```json
{
  "threshold": 35
}
```

Pesan tersebut dikirim ke topic `smartroom/sr_yudhi_20261107/command`.

## 5. Struktur Firmware

Firmware menggunakan pembagian modul:

- `main.ino` – Program utama.
- `Sensor.h / Sensor.cpp` – Pembacaan sensor.
- `Alarm.h / Alarm.cpp` – Pengaturan alarm.
- `Display.h / Display.cpp` – Tampilan OLED.
- `MQTT.h / MQTT.cpp` – Komunikasi MQTT.

## 6. Dashboard

Dashboard dikembangkan menggunakan React Native dan Expo.

Informasi yang ditampilkan:
- Status koneksi MQTT.
- Status perangkat.
- Suhu dan kelembapan.
- Intensitas cahaya.
- Deteksi pergerakan.
- Status buzzer.
- Pengaturan threshold suhu.

## 7. Cara Pengujian

1. Jalankan simulasi ESP32 pada Wokwi.
2. Pastikan ESP32 terhubung ke Wi-Fi dan broker MQTT.
3. Jalankan dashboard.
4. Amati data sensor yang diterima.
5. Uji alarm dengan menaikkan suhu.
6. Uji tombol acknowledge.
7. Uji perubahan threshold melalui MQTT.

## 8. Catatan Implementasi

Proyek dikembangkan menggunakan simulator Wokwi. Pada simulasi digunakan LDR Sensor Module karena keterbatasan ketersediaan komponen. Rancangan sesuai instruksi menggunakan LDR biasa dengan resistor 10 kΩ sebagai pembagi tegangan.

