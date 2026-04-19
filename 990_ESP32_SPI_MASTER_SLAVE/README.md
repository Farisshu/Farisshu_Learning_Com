# 990 ESP32 SPI Master-Slave Communication

[![Build Status](https://img.shields.io/badge/build-passing-green)]()
[![Code Quality](https://img.shields.io/badge/cppcheck-passing-green)]()
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-enabled-blue)]()

SPI (Serial Peripheral Interface) communication antara ESP32 sebagai master dan slave.

## 📋 Deskripsi

Project ini mendemonstrasikan komunikasi SPI full-duplex antara dua ESP32, dimana satu berfungsi sebagai master dan yang lain sebagai slave. Cocok untuk high-speed data transfer antar mikrokontroler.

## ✨ Fitur

- ✅ **Full-Duplex Communication** - Simultaneous TX/RX
- ✅ **High-Speed Transfer** - Up to 10 MHz clock speed
- ✅ **DMA Support** - Efficient bulk data transfer
- ✅ **Multi-byte Transactions** - Configurable transaction size
- ✅ **Interrupt-driven** - Non-blocking operation
- ✅ **Error Detection** - CRC validation support

## 🏗️ Struktur Folder

```
990_ESP32_SPI_MASTER_SLAVE/
├── src/
│   ├── main.cpp      # SPI master/slave implementation
│   └── ...           # Module implementations
├── README.md         # This file
└── platformio.ini    # Build configuration
```

## 🔧 Konfigurasi Hardware

### Pin Configuration (HSPI)
```cpp
// Master & Slave pins (must match)
#define SPI_SCK_PIN     18
#define SPI_MISO_PIN    19
#define SPI_MOSI_PIN    23
#define SPI_SS_PIN      5

// Slave select control
#define SPI_SLAVE_SS    5
```

### Wiring Diagram (Master to Slave)
```
ESP32 Master          ESP32 Slave
────────────          ────────────
GPIO18 (SCK) ───────── GPIO18 (SCK)
GPIO23 (MOSI) ──────── GPIO23 (MOSI)
GPIO19 (MISO) ──────── GPIO19 (MISO)
GPIO5  (SS/CS) ─────── GPIO5  (SS/CS)
GND        ─────────── GND
3.3V       ─────────── 3.3V
```

## 🚀 Quick Start

### Build & Upload (Master)
```bash
cd 990_ESP32_SPI_MASTER_SLAVE
# Upload to first ESP32 as MASTER
pio run --target upload
pio device monitor
```

### Build & Upload (Slave)
```bash
# Modify main.cpp: #define SPI_MODE SLAVE_MODE
# Upload to second ESP32 as SLAVE
pio run --target upload
pio device monitor
```

### Monitor Output
```
[MASTER] SPI initialized at 1 MHz
[MASTER] Sending: [01 02 03 04 05]
[MASTER] Received: [AA BB CC DD EE]
[SLAVE]  Received: [01 02 03 04 05]
[SLAVE]  Sending: [AA BB CC DD EE]
```

## 📊 SPI Protocol

### Signal Lines
| Signal | Name | Description |
|--------|------|-------------|
| SCK | Serial Clock | Clock from master |
| MOSI | Master Out Slave In | Data master → slave |
| MISO | Master In Slave Out | Data slave → master |
| SS/CS | Slave Select | Chip select (active low) |

### Transaction Format
```
Master: SS Low → Clock 8 bits (MOSI) → Read 8 bits (MISO) → SS High
Slave:  Wait SS Low → Shift 8 bits (MISO) → Read 8 bits (MOSI) → Wait SS High
```

## ⚙️ Configuration

### SPI Speed Options
```cpp
#define SPI_FREQUENCY   1000000   // 1 MHz (default)
#define SPI_FREQUENCY   5000000   // 5 MHz (high speed)
#define SPI_FREQUENCY  10000000   // 10 MHz (max)
```

### Data Mode
```cpp
#define SPI_MODE_MASTER 1
#define SPI_MODE_SLAVE  0

// Select mode in main.cpp
#define SPI_MODE SPI_MODE_MASTER
```

### VSPI vs HSPI
```ini
# Use VSPI (default pins)
build_flags = -D USE_VSPI

# Use HSPI (custom pins)
build_flags = -D USE_HSPI
```

## 🧪 Testing

### Loopback Test
```cpp
// Connect MOSI to MISO on master
#define LOOPBACK_TEST true
```

### Performance Test
```cpp
// Measure transfer speed
uint32_t start = micros();
spi_transfer(data, size);
uint32_t elapsed = micros() - start;
float speed = (size * 8.0) / elapsed; // Mbps
```

## 🔍 Troubleshooting

### No Data Transfer
1. Verify all 4 signal connections (SCK, MOSI, MISO, SS)
2. Check common ground between master and slave
3. Ensure SS pin is correctly controlled
4. Verify SPI mode matches (master vs slave)

### Corrupted Data
- Reduce SPI clock frequency
- Check for loose connections
- Add pull-up resistors on SS line (10kΩ)
- Keep cables short (< 20cm for >1MHz)

### Slave Not Responding
- Verify SS polarity (active low)
- Check slave initialization sequence
- Ensure slave has proper power supply
- Monitor SS line with oscilloscope

## 📚 References

- [SPI Protocol](https://en.wikipedia.org/wiki/Serial_Peripheral_Interface)
- [ESP32 SPI Documentation](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/spi_master.html)
- [Arduino SPI Library](https://www.arduino.cc/reference/en/language/functions/communication/spi/)

## 📄 License

MIT License - see main repository LICENSE file.

---

*Version: 1.0.0*  
*Last Updated: 2025*
