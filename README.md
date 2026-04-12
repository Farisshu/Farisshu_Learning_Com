# 📦 Farisshu Embedded Portfolio
Kumpulan proyek ESP32 + TFT ST7735 untuk persiapan magang Embedded Engineer.

---

## 📂 Struktur Proyek
| Folder | Deskripsi | Status |
|--------|-----------|--------|
| `01_motor_monitor/` | Simulasi RPM + progress bar + CLI serial | ✅ Stabil |
| `02_waveform_graph/` | Real-time scrolling chart | 🚧 Development |
| `03_industrial_hmi/` | UI industrial + alarm + dirty-flag rendering | 📋 Planned |

---

## 🔌 01 Motor Monitor
Simulasi dashboard RPM motor dengan transisi smooth, auto-mode, dan command-line interface via Serial.

### ✨ Fitur
- 🎨 Full-screen gradient UI
- 📊 Progress bar dinamis (Hijau → Kuning → Merah)
- 🔄 Auto-simulation (RPM berubah natural tiap 5-10 detik)
- ⌨️ CLI Serial: `rpm <nilai>`, `auto`, `status`, `help`
- 📦 Modular code: `display.h/cpp` + `simulator.h/cpp`

### 🔧 Wiring (ESP32 → ST7735)
| TFT Pin | ESP32 |
|---------|-------|
| `VCC`   | `3V3` |
| `GND`   | `GND` |
| `SCK`   | `GPIO 18` |
| `SDA`   | `GPIO 23` |
| `CS`    | `GPIO 5` |
| `DC`    | `GPIO 17` |
| `RST`   | `GPIO 4` |
| `BL`    | `GPIO 2` |

### 🚀 Cara Build & Upload
1. Buka folder `01_motor_monitor` di VS Code
2. Pastikan extension **PlatformIO IDE** terinstall
3. Klik **Build** (`Ctrl+Alt+B`)
4. Klik **Upload** (`Ctrl+Alt+U`)
5. Buka **Serial Monitor** (`Ctrl+Shift+M`) → baud `115200`
6. Ketik `auto` atau `rpm 1500`

### 📸 Demo
*(Screenshot/GIF akan ditambahkan di sini)*
