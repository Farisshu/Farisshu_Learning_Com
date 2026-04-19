# 05 RS485 Master Node

[![Build Status](https://img.shields.io/badge/build-passing-green)]()
[![Code Quality](https://img.shields.io/badge/cppcheck-passing-green)]()
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-enabled-blue)]()

RS485 communication master node untuk industrial networking dengan ESP32.

## 📋 Deskripsi

Project ini mengimplementasikan RS485 master node yang berkomunikasi dengan slave nodes menggunakan protokol serial asynchronous. Cocok untuk aplikasi industrial monitoring dan control systems.

## ✨ Fitur

- ✅ **Non-blocking Communication** - Menggunakan FreeRTOS tasks untuk komunikasi async
- ✅ **Protocol Parser** - Validasi dan parsing data packets
- ✅ **Noise Filtering** - Filter karakter printable untuk noise rejection
- ✅ **Link Monitoring** - Deteksi koneksi slave dengan timeout
- ✅ **Logging System** - Comprehensive logging untuk debugging
- ✅ **Error Handling** - Robust error handling dengan NULL checks

## 🏗️ Struktur Folder

```
05_rs485_master/
├── include/          # Public headers
├── src/
│   ├── main.cpp      # Entry point & master logic
│   └── ...           # Module implementations
├── README.md         # This file
└── platformio.ini    # Build configuration
```

## 🔧 Konfigurasi Hardware

### Pin Configuration (Default)
```cpp
#define RS485_RX_PIN    16
#define RS485_TX_PIN    17
#define RS485_BAUD      9600
```

### Wiring Diagram
```
ESP32          RS485 Module
─────          ────────────
GPIO16 (RX) ─── RO
GPIO17 (TX) ─── DI
5V        ──── VCC
GND       ──── GND
GPIOxx    ──── DE/RE (optional, for flow control)
```

## 🚀 Quick Start

### Build & Upload
```bash
cd 05_rs485_master
pio run --target upload
pio device monitor
```

### Monitor Output
```
[INFO] RS485 Master initialized
[INFO] Scanning for slaves...
[INFO] Slave 1 connected: T=25.5°C, P=1013.2hPa
[WARN] Slave 2 timeout
[INFO] Link alive: true
```

## 📊 Protocol Format

Master mengirim command dan menerima response dengan format:
```
Request:  "CMD|READ|SENSOR\n"
Response: "DATA|T:25.50|P:1013.20|END\n"
```

### Field Description
- `DATA` - Protocol header
- `T:xx.xx` - Temperature value (°C)
- `P:xxxx.xx` - Pressure value (hPa)
- `END` - Packet terminator

## ⚙️ Configuration

Edit `platformio.ini` untuk customize:

```ini
[env:esp32dev]
board = esp32dev
framework = arduino
build_flags = 
    -D CORE_DEBUG_LEVEL=3
    -D LOG_LEVEL=3
```

## 🧪 Testing

### Unit Tests
```bash
pio test
```

### Static Analysis
```bash
cppcheck --enable=all --std=c++11 src/*.cpp
```

## 🔍 Troubleshooting

### No Response from Slaves
1. Check wiring (RX/TX swapped?)
2. Verify baud rate matches slave configuration
3. Check DE/RE pin control if using flow control
4. Ensure proper termination resistors (120Ω)

### Noise Issues
- Use shielded cable
- Add termination resistors
- Enable noise filtering in code
- Reduce cable length

### Memory Issues
- Monitor heap with `xPortGetFreeHeapSize()`
- Adjust task stack sizes in `config.h`
- Reduce buffer sizes if needed

## 📚 References

- [RS485 Communication Protocol](https://en.wikipedia.org/wiki/RS-485)
- [ESP32 UART Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/uart.html)
- [FreeRTOS Task Management](https://www.freertos.org/taskmanagement.html)

## 📄 License

MIT License - see main repository LICENSE file.

---

*Version: 1.0.0*  
*Last Updated: 2025*
