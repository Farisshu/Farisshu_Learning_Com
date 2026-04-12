# 🚗 01 Motor Monitor
Simulasi dashboard RPM dengan ESP32 + TFT ST7735.

## ✨ Fitur
- Full-screen gradient UI (landscape 160x128)
- Progress bar dinamis (Hijau→Kuning→Merah)
- Auto-simulation: RPM berubah natural tiap 5-10 detik
- CLI Serial: `rpm <nilai>`, `auto`, `status`, `help`
- Modular code: `display.h/cpp` + `simulator.h/cpp`

## 🔌 Wiring
| TFT Pin | ESP32 GPIO |
|---------|-----------|
| VCC     | 3V3       |
| GND     | GND       |
| SCK     | 18        |
| SDA     | 23        |
| CS      | 5         |
| DC      | 17        |
| RST     | 4         |
| BL      | 2         |

## 🚀 Build & Upload
1. Buka folder ini di VS Code
2. `Ctrl+Alt+B` → Build
3. `Ctrl+Alt+U` → Upload
4. Serial Monitor 115200 baud

## ⌨️ Command Serial