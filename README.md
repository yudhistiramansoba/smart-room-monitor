# Smart Room Monitor

Smart Room Monitor merupakan sistem pemantauan kondisi ruangan berbasis ESP32 yang menggunakan sensor suhu, kelembapan, intensitas cahaya, dan gerakan. Data sensor dikirim menggunakan protokol MQTT dan ditampilkan melalui Smart Room Dashboard.

## 1. Komponen yang Digunakan

- ESP32
- Sensor DHT22
- Sensor LDR
- Sensor PIR HC-SR501
- OLED SSD1306 I2C
- LED hijau dan LED merah
- Buzzer
- Push Button
- Resistor sesuai rangkaian
- Breadboard dan kabel jumper

## 2. Tabel Pin ESP32

| Komponen | Pin ESP32 |
|---|---|
| DHT22 (DATA) | GPIO 4 |
| LDR (OUT) | GPIO 34 |
| PIR (OUT) | GPIO 27 |
| LED Hijau | GPIO 25 |
| LED Merah | GPIO 26 |
| Buzzer | GPIO 33 |
| Push Button | GPIO 14 |
| OLED SDA | GPIO 21 |
| OLED SCL | GPIO 22 |
| OLED I2C Address | `0x3C` |

## 3. Build dan Flash Firmware

1. Install Arduino IDE.
2. Install library yang dibutuhkan.
3. Buka file `Smart Room.ino` melalui Arduino IDE.
4. Pilih board ESP32 dan port yang sesuai.
5. Klik **Verify** untuk memeriksa program.
6. Klik **Upload** untuk mengunggah firmware ke ESP32.

Seluruh file pendukung `.h` dan `.cpp` harus berada dalam satu folder proyek firmware.

### Library yang Dibutuhkan

- DHT sensor library
- Adafruit SSD1306
- Adafruit GFX Library
- PubSubClient
- ArduinoJson

## 4. Konfigurasi MQTT

Sistem menggunakan broker publik HiveMQ untuk komunikasi antara ESP32 dan dashboard.

| Konfigurasi | Nilai |
|---|---|
| Broker | `broker.hivemq.com` |
| MQTT TCP Port | `1883` |
| MQTT WebSocket Port | `8000` |
| WebSocket Path | `/mqtt` |
| Device ID | `sr_yudhi_20261107` |

### MQTT Topics

| Topic | Fungsi |
|---|---|
| `smartroom/sr_yudhi_20261107/telemetry` | Mengirim data sensor |
| `smartroom/sr_yudhi_20261107/status` | Status koneksi perangkat |
| `smartroom/sr_yudhi_20261107/command` | Menerima perintah perubahan threshold |
| `smartroom/sr_yudhi_20261107/event` | Mengirim informasi perubahan status alarm |

### Contoh Perintah MQTT

Untuk mengubah ambang batas suhu, kirim pesan JSON berikut ke topic command:

```json
{
  "threshold": 35
}
```

## 5. Menjalankan Smart Room Dashboard

Buka terminal, kemudian masuk ke folder `dashboard`:

```bash
cd dashboard
```

Install dependency:

```bash
npm install
```

Jalankan aplikasi:

```bash
npm start
```

Untuk menjalankan dashboard melalui browser, tekan tombol `w` pada terminal Expo.

## 6. Schematic dan Simulasi

File rangkaian dan simulasi tersedia pada direktori berikut:

- `schematic/Schematic.fz` – File rangkaian Fritzing.
- `schematic/schematic fritzing.png` – Gambar rangkaian Fritzing.
- `schematic/schematic wokwi.png` – Gambar rangkaian simulasi Wokwi.
- `schematic/wokwi-link.txt` – Tautan simulasi Wokwi.

## 7. Demo

Video demonstrasi sistem tersedia pada:

`demo/demo-smart-room.mp4`

## 8. Design Notes

Dokumentasi keputusan desain, kendala selama pengerjaan, dan rencana pengembangan tersedia pada:

`design-notes/design-notes.md`
