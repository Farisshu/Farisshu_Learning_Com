# 📈 02 Waveform Graph
Real-time scrolling oscilloscope simulation using ESP32 + TFT ST7735.

## ✨ Fitur
- **Scrolling Chart**: Gelombang bergerak real-time (50 FPS)
- **Signal Control**: Atur frekuensi dan amplitudo via Serial
- **Pause/Resume**: Freeze grafik untuk screenshot
- **Noise Simulation**: Penambahan noise acak untuk realisme
- **Modular Code**: Pemisahan file `graph.cpp` (data) dan `display.cpp` (UI)

## 🔌 Wiring
Sama seperti proyek 01 (ST7735 SPI Standard)

## 🚀 Build & Upload
1. Buka folder di VS Code
2. `Ctrl+Alt+B` (Build)
3. `Ctrl+Alt+U` (Upload)
4. Serial Monitor 115200 baud

## ⌨️ Command Serial