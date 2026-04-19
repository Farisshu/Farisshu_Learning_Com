# 01 Motor Monitor - ESP32 TFT Display

## 📋 Deskripsi
Proyek simulasi monitor motor RPM dengan display TFT ST7735. Menampilkan visualisasi grafis RPM motor yang berubah secara real-time dengan kontrol melalui serial command.

## 🎯 Fitur Utama
- ✅ Display TFT ST7735 (128x160) dengan library TFT_eSPI
- ✅ Simulasi RPM motor dengan perubahan realistis
- ✅ Serial command interface untuk kontrol interaktif
- ✅ Non-blocking design pattern
- ✅ LED indicator pada GPIO 2

## 📁 Struktur Folder
```
01_motor_monitor/
├── include/          # Header files (jika ada)
├── lib/              # Library eksternal custom (jika ada)
├── src/
│   ├── main.cpp      # Entry point aplikasi
│   ├── display.h     # Header display interface
│   ├── display.cpp   # Implementasi display driver
│   ├── simulator.h   # Header simulator engine
│   └── simulator.cpp # Implementasi RPM simulator
├── test/             # Unit tests (Unity framework)
└── platformio.ini    # Konfigurasi PlatformIO
```

## 🔧 Konfigurasi Hardware
| Komponen | Pin ESP32 | Keterangan |
|----------|-----------|------------|
| TFT CS   | GPIO 5    | Chip Select |
| TFT DC   | GPIO 17   | Data/Command |
| TFT MOSI | GPIO 23   | SPI Data |
| TFT SCLK | GPIO 18   | SPI Clock |
| TFT RST  | GPIO 4    | Reset |
| TFT BL   | GPIO 2    | Backlight |
| LED      | GPIO 2    | Status Indicator |

## 🚀 Cara Menggunakan

### Build & Upload
```bash
# Build project
pio run

# Upload ke ESP32
pio run --target upload

# Serial monitor
pio device monitor
```

### Serial Commands
```
help        - Tampilkan daftar command
setrpm X    - Set RPM ke nilai X (contoh: setrpm 1500)
status      - Tampilkan status saat ini
reset       - Reset simulator ke default
```

## 🏗️ Arsitektur Software
```
┌─────────────────┐     ┌─────────────────┐
│   main.cpp      │────▶│   display.cpp   │
│  (Application   │     │  (TFT Driver)   │
│     Logic)      │     └─────────────────┘
└────────┬────────┘
         │
         ▼
┌─────────────────┐
│  simulator.cpp  │
│ (RPM Generator) │
└─────────────────┘
```

## 📝 Catatan Pengembangan
- Update interval: 100ms (10 FPS)
- Menggunakan non-blocking timer pattern
- Compatible dengan ESP32 DevKit V1

## 🔗 Referensi
- [TFT_eSPI Library](https://github.com/Bodmer/TFT_eSPI)
- [PlatformIO ESP32](https://docs.platformio.org/en/latest/platforms/espressif32.html)

---
**Author**: M Faris Asroru Ghifary  
**License**: MIT
