# 🚗 CAN Bus Simulation Node (ESP32 + MCP2515/TJA1050)

Simulasi sistem komunikasi vehicle network menggunakan protokol CAN Bus 2.0B (Standard/Extended ID) pada kecepatan 250kbps. Proyek ini mendemonstrasikan arsitektur terdistribusi: **Master (Transmitter)** mengirim data sensor simulasi, **Slave (Receiver)** mem-parse dan memvalidasi frame.

> 🎯 **Target Kompetensi**: Embedded Engineer (Automotive/Industrial Measurement)  
> 🏢 **Relevansi**: Horiba Software Design Dept, Engine Test Systems, ECU Communication

---

## 🏗️ Arsitektur Sistem

[ESP32 Master] ──(SPI)── [MCP2515+TJA1050] ═══(CAN_H/L)═══ [MCP2515+TJA1050] ──(SPI)── [ESP32 Slave]
(Sensor Sim) (5V, J1=ON) Differential Bus (5V, J1=ON) (Parser + Debug)


### 🔄 Alur Data
1. Master generate data simulasi: `RPM`, `Coolant Temp`, `Status Flag`
2. Packing ke CAN Frame: ID `0x100`, 8-byte payload, Big-Endian encoding
3. Transmit via CAN Bus @250kbps
4. Slave receive, filter ID `0x100`, parse payload, validasi checksum implicit
5. Output via Serial Monitor + timeout detection (link loss monitoring)

---

## 🔌 Wiring Diagram

### 📡 Common untuk Master & Slave
| Komponen | Pin Modul | → ESP32 | Catatan |
|----------|-----------|---------|---------|
| **Power** | `VCC` | `5V` | ⚠️ TJA1050 wajib 5V (bukan 3.3V) |
| | `GND` | `GND` | **WAJIB Common Ground** antar semua device |
| **SPI** | `SCK` | `GPIO 18` | Shared bus |
| | `SI` (MOSI) | `GPIO 23` | Shared bus |
| | `SO` (MISO) | `GPIO 19` | ⚠️ Gunakan resistor 1kΩ seri jika modul 5V → ESP32 3.3V |
| | `CS` | `GPIO 5` (Master) / `GPIO 22` (Slave) | Chip Select unik per device |
| **CAN Bus** | `CAN_H` | ↔ `CAN_H` node lain | Twisted pair recommended, <1m untuk tes |
| | `CAN_L` | ↔ `CAN_L` node lain | Jangan disilang! |
| | `J1` | **ON / Short** | 120Ω termination resistor (wajib di 2 node ujung) |

### ⚠️ Critical Notes
- **Logic Level**: ESP32 GPIO = 3.3V, TJA1050 I/O = 5V. Gunakan:
  - ✅ Resistor 1kΩ seri di jalur `SO→GPIO19` (minimal proteksi), atau
  - ✅ Logic Level Converter bi-directional (rekomendasi produksi)
- **Common Ground**: Tanpa GND common, sinyal diferensial tidak punya referensi → komunikasi gagal.
- **Termination**: Untuk 2-node bus, **kedua modul wajib J1 ON**. Node tambahan di tengah: J1 OFF.

---

## 📜 Protokol Komunikasi

### Frame Specification
| Field | Value | Deskripsi |
|-------|-------|-----------|
| **CAN ID** | `0x100` (Standard) | Filter di receiver |
| **DLC** | `8 bytes` | Payload penuh |
| **Baudrate** | `250 kbps` | Standar otomotif ringan (CAN Low-Speed) |
| **Encoding** | Big-Endian (Motorola) | Byte 0 = MSB |

### Payload Mapping (Big-Endian)

Byte 0-1 : RPM_raw (uint16_t) → RPM = RPM_raw / 10.0 [700.0 - 6000.0]
Byte 2-3 : Temp_raw (uint16_t) → Temp = Temp_raw / 10.0 [75.0 - 105.0 °C]
Byte 4 : Status Flag (uint8_t)
0x00 = OK (Normal)
0x01 = WARN (Temp > 95°C)
0x02 = CRIT (Temp > 100°C)
Byte 5-7 : Reserved (0x00)


### Contoh Frame

ID: 0x100 | Data: 1F 90 03 52 00 00 00 00
→ RPM_raw = 0x1F90 = 8080 → RPM = 808.0
→ Temp_raw = 0x0352 = 850 → Temp = 85.0°C
→ Status = 0x00 → OK


---

## 🛡️ Safety & Best Practices

| Praktik | Implementasi | Alasan Engineering |
|---------|-------------|-------------------|
| **Non-Blocking Design** | `millis()`-based timing, no `delay()` blocking | Responsif, siap integrasi RTOS |
| **Timeout Detection** | Slave monitor lastRx time → flag LINK LOST jika >3s | Fail-safe, deteksi disconnect |
| **Input Validation** | Filter ID `0x100`, cek DLC ≥5 sebelum parse | Mencegah corrupt data processing |
| **Power Integrity** | Modul CAN @5V, ESP32 @3.3V, level shifting di MISO | Proteksi hardware, sinyal stabil |
| **Bus Termination** | 120Ω di kedua ujung bus (J1 ON) | Mencegah refleksi sinyal, noise immunity |
| **Common Ground** | Single-point GND reference | Referensi tegangan konsisten untuk differential signaling |

---

## 🚀 Build & Deploy

### Prerequisites
- PlatformIO for VS Code
- ESP32 Dev Module (2 unit)
- MCP2515 + TJA1050 CAN Module (2 unit)
- Jumper wires, resistor 1kΩ (opsional level shifter)

### Langkah
```bash
# 1. Clone repo
git clone https://github.com/Farisshu/Farisshu_Learning_Com.git
cd Farisshu_Learning_Com/07_can_bus_node

# 2. Upload Master
#    - Buka folder master/ di PlatformIO
#    - Upload via USB (esptool)
#    - Serial Monitor: baud 115200

# 3. Upload Slave
#    - Buka folder slave/ di PlatformIO
#    - Upload via USB
#    - Serial Monitor: baud 115200

# 4. Verifikasi
#    Master: [TX] ✅ ID:0x100 | RPM:xxx | TEMP:xx.xC
#    Slave : [RX] ✅ ID:0x100 | RPM:xxx | TEMP:xx.xC | STATUS:0xXX

Troubleshooting Cepat
Gejala
Kemungkinan
Solusi
TX Failed (No ACK)
Slave tidak ACK / Bus Off
Cek: GND common, CAN_H/L polarity, J1 ON, Slave dalam MCP_NORMAL
RX garbage / wrong ID
Noise / ID filter salah
Cek: terminasi 120Ω, level shifting MISO, filter ID di kode
CAN init failed
SPI wiring / power / crystal
Cek: SCK/MOSI/MISO/CS, VCC 5V stabil, crystal 8MHz
Link Lost terus
Timeout / kabel putus
Cek: continuity CAN_H/L, jarak <1m, hindari breadboard di jalur CAN

📂 Struktur Kode
master/src/main.cpp
Inisialisasi CAN @250kbps, 8MHz crystal
Simulasi sensor: RPM (700-6000), Temp (75-105°C), Status flag
Packing Big-Endian, transmit ID 0x100 tiap 1 detik
Recovery routine jika masuk Bus-Off state
slave/src/main.cpp
Inisialisasi CAN, filter ID 0x100
Parsing payload: RPM, Temp, Status dengan scaling x10
Timeout detection: flag LINK LOST jika >3s tanpa data
Debug output: raw hex + parsed value via Serial

🔗 Referensi & Standar
CAN Bus Specification v2.0B
TJA1050 Datasheet (NXP)
MCP2515 Datasheet (Microchip)
ISO 11898-1:2015 (Road vehicles — Controller area network (CAN) — Part 1: Data link layer)
Author: M Faris Asroru Ghifary
Contact: m.farisasrorughifary@gmail.com | LinkedIn
Last Updated: April 2026
License: MIT (Educational Purpose)

