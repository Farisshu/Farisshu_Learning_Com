# 🏭 03 Industrial HMI
Real-time industrial monitoring dashboard with alarm state machine using ESP32 + TFT ST7735.

## ✨ Fitur
- **Dual Monitoring**: Suhu & Tekanan real-time dengan progress bar dinamis
- **Alarm State Machine**: Otomatis switch `Normal ↔ Alarm` saat melewati threshold
- **UI Optimization**: Rendering non-blocking (`millis`), anti-ghosting text, tanpa `delay()`
- **Flashing Alert**: Layar kedip merah/hitam saat kondisi CRITICAL
- **Modular Architecture**: Pemisahan ketat `ui/` (tampilan) & `logic/` (data & logika)
- **Serial CLI**: Simulasi sensor & kontrol real-time via terminal

## 🔌 Wiring
Sama seperti proyek sebelumnya (ST7735 SPI Standard)

## 🚀 Build & Upload
1. Buka folder di VS Code
2. `Ctrl+Alt+B` (Build)
3. `Ctrl+Alt+U` (Upload)
4. Serial Monitor 115200 baud

## ⌨️ Command Serial


## 🧠 Konsep Teknikal (Interview Ready)
- **State Machine**: Alur logika `Normal State` ↔ `Alarm State` berdasarkan threshold, bukan polling manual
- **Non-Blocking UI**: Menggunakan `millis()` timer untuk refresh layar → Serial tetap responsif, tidak ada freeze
- **Anti-Ghosting**: Kombinasi `setTextColor(FG, BG)` + `fillRect` presisi → text tidak menumpuk saat update cepat
- **Separation of Concerns**: `logic.cpp` hanya kalkulasi data, `ui.cpp` hanya rendering. Mudah di-porting atau di-unit test

## 📸 Demo
*(Drag & drop screenshot/GIF di sini)*