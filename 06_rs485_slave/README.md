# 📡 RS485 Industrial Monitoring Node (ESP32)
Simulasi sistem SCADA/PLC terdistribusi menggunakan komunikasi RS485 Modbus-like protocol.

## 🏗️ Arsitektur Sistem

[ESP32 Master] ──(RS485 A+/B-)── [ESP32 Slave + TFT ST7735]
(Sensor Sim) (Parser + UI + WDT)


## 🔌 Wiring Diagram
| Komponen | Pin | Koneksi |
|----------|-----|---------|
| **RS485 Module** | `VCC` | `3.3V` |
| | `GND` | `GND` (Common Ground wajib) |
| | `RXD` | `GPIO 16` |
| | `TXD` | `GPIO 17` |
| | `A+` | `A+` ( twisted pair ) |
| | `B-` | `B-` ( twisted pair ) |
| **TFT ST7735** | `SCK` | `GPIO 18` |
| | `MOSI` | `GPIO 23` |
| | `CS` | `GPIO 5` |
| | `DC` | `GPIO 2` |
| | `RST` | `GPIO 4` |
| | `BL` | `3.3V` |

## 📜 Protokol Komunikasi

Frame: DATA|T:<float>|P:<float>|END\n
Example: DATA|T:25.40|P:1.20|END
Baudrate: 9600 | Parity: None | Stop: 1
Timeout: 5000ms (auto OFFLINE detection)


## 🛡️ Safety & Standards
- ✅ **Hardware Watchdog**: 8s timeout, auto-recovery on freeze
- ✅ **Non-Blocking Architecture**: `millis()`-based UI & UART polling
- ✅ **Noise Immunity**: Printable char filter + header validation + buffer overflow guard
- ✅ **MISRA-Friendly**: No dynamic allocation, explicit types, const-correct, deterministic timing
- ✅ **Fail-Safe UI**: Offline state indication, safe redraw boundaries

## 🚀 Build & Deploy
```bash
# 1. Clone & open in VS Code + PlatformIO
# 2. Install dependencies: bodmer/TFT_eSPI@^2.5.43
# 3. Upload via cable (esptool) or OTA
pio run --target upload

📂 Struktur Kode
src/
├── main.cpp           ← Orchestrator + WDT management
├── ui_display.cpp     ← TFT rendering (layout konstan, anti-cut)
└── rs485_parser.cpp   ← UART polling, sscanf parsing, timeout detection

Author: M Faris Asroru Ghifary
Target: Embedded Engineer Internship (Horiba / Automotive-Industrial)
Last Updated: April 2026