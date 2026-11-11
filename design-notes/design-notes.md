# Smart Room Monitor – Design Notes

## 1. Keputusan Desain

Proyek Smart Room Monitor menggunakan ESP32 sebagai mikrokontroler utama karena mendukung konektivitas Wi-Fi dan komunikasi MQTT. Sensor DHT22 digunakan untuk membaca suhu dan kelembapan, LDR untuk mendeteksi intensitas cahaya, serta PIR untuk mendeteksi gerakan.

OLED SSD1306 digunakan untuk menampilkan informasi sensor secara langsung. LED hijau dan merah serta buzzer berfungsi sebagai indikator dan alarm ketika suhu melewati ambang batas. Tombol digunakan untuk melakukan acknowledge atau mematikan alarm.

Komunikasi antara ESP32 dan dashboard menggunakan protokol MQTT. Dashboard dibuat menggunakan React Native dan Expo untuk memantau data sensor serta mengirim perintah perubahan ambang batas suhu.

## 2. Masalah yang Ditemui

Selama pengerjaan, terdapat beberapa kendala, terutama dalam penyesuaian pembacaan sensor dan integrasi komunikasi MQTT antara ESP32 dan dashboard.

Salah satu kendala adalah tombol acknowledge yang sebelumnya hanya diperiksa setiap lima detik, mengikuti interval pembacaan sensor. Hal ini menyebabkan respons tombol kurang cepat. Masalah tersebut diperbaiki dengan memeriksa tombol dan status alarm secara terus-menerus di dalam fungsi loop.

Selain itu, terdapat perbedaan komponen LDR pada simulasi Wokwi dengan rancangan awal. Simulasi menggunakan modul LDR, sedangkan rancangan awal menggunakan LDR dan resistor 10 kΩ sebagai pembagi tegangan.

## 3. Pengembangan Selanjutnya

Jika tersedia waktu lebih, pengembangan berikutnya adalah melakukan pengujian menggunakan perangkat keras secara langsung, meningkatkan kestabilan koneksi MQTT, serta menyempurnakan tampilan dashboard.

Selain itu, sistem dapat dikembangkan dengan menambahkan penyimpanan riwayat data sensor dan fitur pemantauan melalui internet secara lebih optimal.