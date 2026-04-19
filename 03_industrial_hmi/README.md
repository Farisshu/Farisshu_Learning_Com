# 🏭 03 Industrial HMI - ESP32 TFT Human Machine Interface

## 📋 Deskripsi
Sistem monitoring industri berbasis ESP32 dengan tampilan TFT untuk menampilkan data sensor (suhu dan tekanan) secara real-time. Dilengkapi dengan sistem alarm visual ketika nilai sensor melebihi batas aman.

## ✨ Fitur
- ✅ Display TFT ST7735 (128x160) dengan UI yang responsif
- ✅ Simulasi data sensor suhu dan tekanan
- ✅ Sistem alarm visual dengan layar berkedip
- ✅ Command parsing via Serial untuk testing
- ✅ Non-blocking design (tanpa delay panjang)
- ✅ FreeRTOS compliant coding standard
- ✅ Logging system terstruktur
- ✅ Error handling yang robust

## 🏗️ Struktur Folder
```
03_industrial_hmi/
├── include/              # Header files (shared)
├── lib/                  # External libraries
├── src/
│   ├── main.cpp          # Entry point & main loop
│   ├── ui.h              # UI module header
│   ├── ui.cpp            # UI rendering implementation
│   ├── logic.h           # Logic module header
│   └── logic.cpp         # Sensor logic & alarm management
├── test/                 # Unit tests (Unity framework)
├── README.md             # Dokumentasi ini
└── platformio.ini        # Konfigurasi PlatformIO
```

## 🔧 Komponen yang Digunakan
| Komponen | Pin ESP32 | Keterangan |
|----------|-----------|------------|
| TFT ST7735 | MOSI: 23 | SPI Data |
| | SCLK: 18 | SPI Clock |
| | CS: 5 | Chip Select |
| | DC: 17 | Data/Command |
| | RST: 4 | Reset |
| | BL: 2 | Backlight |
| | VCC | 3.3V |
| | GND | Ground |

## 🚀 Cara Build & Upload

### Prerequisites
- PlatformIO IDE (VS Code extension)
- ESP32 board driver terinstall

### Langkah-langkah
```bash
# 1. Navigate ke folder project
cd 03_industrial_hmi

# 2. Build project
pio run

# 3. Upload ke ESP32
pio run --target upload

# 4. Buka Serial Monitor
pio device monitor
```

### Code Quality Check
```bash
# Run cppcheck dan clang-tidy
pio check
```

### Unit Testing
```bash
# Run unit tests
pio test
```

## 📡 Command Serial
Ketik command berikut di Serial Monitor (baud 115200):

| Command | Fungsi | Contoh |
|---------|--------|--------|
| `temp <nilai>` | Set simulasi suhu | `temp 70` |
| `press <nilai>` | Set simulasi tekanan | `press 5.5` |

## 🎮 Cara Penggunaan
1. Upload code ke ESP32
2. Buka Serial Monitor (115200 baud)
3. Lihat tampilan di TFT:
   - **Temperature**: Bar kuning dengan nilai real-time
   - **Pressure**: Bar hijau dengan nilai real-time
   - **Status**: NORMAL (hijau) atau CRITICAL (merah)
4. Tes alarm dengan mengetik: `temp 70` atau `press 6`
5. Layar akan berkedip merah dengan tulisan "ALARM"

## 🏛️ Arsitektur Software

### Module Diagram
```
┌─────────────────────────────────────────┐
│              main.cpp                   │
│  - setup(): Initialize modules          │
│  - loop(): Main execution cycle         │
│  - processSerialCommand(): UART handler │
└──────────────┬──────────────────────────┘
               │
       ┌───────┴────────┐
       │                │
       ▼                ▼
┌─────────────┐  ┌─────────────┐
│   ui.cpp    │  │  logic.cpp  │
│ - initUI()  │  │ - initLogic │
│ - render... │  │ - update... │
│ - drawBar() │  │ - check...  │
│             │  │ - getData() │
└─────────────┘  └─────────────┘
```

### Task Flow (Non-Blocking)
```
setup()
  ├─ init Serial (115200)
  ├─ initUI() → STATUS_OK
  └─ initLogic() → STATUS_OK

loop() (setiap ~100ms)
  ├─ updateSensorData()
  ├─ checkAlarmStatus()
  ├─ processSerialCommand()
  └─ renderDashboard() / renderAlarm()
```

## 📊 State Machine
```
[NORMAL STATE]
  ├─ Display: Dashboard view
  ├─ Temperature bar (yellow)
  ├─ Pressure bar (green)
  └─ Status: "NORMAL"
       │
       │ (temp > 60°C OR pressure > 5 Bar)
       ▼
[ALARM STATE]
  ├─ Display: Blinking red screen
  ├─ Text: "!!! ALARM CHECK SENSORS"
  └─ Blink rate: 300ms
```

## ⚙️ Konfigurasi
Edit `platformio.ini` untuk mengubah:
- **TFT pins**: Sesuaikan dengan wiring
- **MAX_TEMP**: Batas suhu alarm (default: 60.0°C)
- **MAX_PRESS**: Batas tekanan alarm (default: 5.0 Bar)
- **UI_UPDATE_INTERVAL**: Refresh rate display (default: 100ms)

## 🐛 Troubleshooting

### TFT tidak menyala
- Cek koneksi pin (terutama CS, DC, RST)
- Pastikan backlight pin (BL) mendapat 3.3V
- Verifikasi konfigurasi di `platformio.ini`

### Garis-garis pada display
- Tingkatkan SPI frequency jika terlalu lambat
- Turunkan SPI frequency jika ada noise
- Cek kualitas kabel/koneksi

### Alarm tidak trigger
- Pastikan command format benar (`temp 70` bukan `temp=70`)
- Cek nilai threshold di `main.cpp`
- Verifikasi Serial monitor baudrate (115200)

## 📝 Coding Standards
Project ini mengikuti standar:
- **FreeRTOS V10.x Coding Standard**
- **Doxygen Documentation**
- **C++11 with Arduino Framework**
- **Static Analysis**: cppcheck, clang-tidy

### Naming Convention
- `PascalCase`: Types, structs, enums (e.g., `SensorData_t`)
- `camelCase`: Functions, variables (e.g., `initUI`, `currentData`)
- `UPPER_CASE`: Constants, macros (e.g., `MAX_TEMP`, `STATUS_OK`)

## 📄 License
MIT License - Silakan digunakan untuk pembelajaran dan proyek pribadi.

## 👥 Author
Farisshu Embedded Project

---
**Last Updated**: 2024
**Firmware Version**: v1.0.0
