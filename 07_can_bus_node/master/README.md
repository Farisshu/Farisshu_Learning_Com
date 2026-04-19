# 07 CAN Bus Master Node

[![Build Status](https://img.shields.io/badge/build-passing-green)]()
[![Code Quality](https://img.shields.io/badge/cppcheck-passing-green)]()
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-enabled-blue)]()

CAN Bus master node untuk automotive dan industrial networking dengan ESP32.

## 📋 Deskripsi

Project ini mengimplementasikan CAN Bus master node yang berkomunikasi dengan slave nodes dalam network. Mendukung standard CAN 2.0B (11-bit dan 29-bit ID).

## ✨ Fitur

- ✅ **CAN 2.0B Support** - 11-bit dan 29-bit identifier
- ✅ **Multi-node Communication** - Broadcast dan point-to-point messaging
- ✅ **Error Detection** - CRC, ACK, dan error frame handling
- ✅ **Non-blocking TX/RX** - FreeRTOS queue-based message handling
- ✅ **Logging System** - Comprehensive message logging
- ✅ **Error Handling** - Robust error recovery

## 🏗️ Struktur Folder

```
master/
├── src/
│   ├── main.cpp      # CAN master implementation
│   └── ...           # Module implementations
├── README.md         # This file
└── platformio.ini    # Build configuration
```

## 🔧 Konfigurasi Hardware

### Pin Configuration (ESP32)
```cpp
#define CAN_TX_PIN    5
#define CAN_RX_PIN    4
#define CAN_BAUD      500000  // 500 kbps
```

### Wiring Diagram
```
ESP32          CAN Transceiver (MCP2551/TJA1050)
─────          ─────────────────────────────────
GPIO5 (TX) ──── TXD
GPIO4 (RX) ──── RXD
3.3V       ──── VCC
GND        ──── GND
CAN_H      ──── H (to CAN bus)
CAN_L      ──── L (to CAN bus)
```

### CAN Bus Termination
```
Node 1 ----[120Ω]---- Node 2 ----[120Ω]---- Node 3
 (Master)              (Slave)              (Slave)
```

## 🚀 Quick Start

### Build & Upload
```bash
cd 07_can_bus_node/master
pio run --target upload
pio device monitor
```

### Monitor Output
```
[INFO] CAN Bus Master initialized
[INFO] CAN speed: 500 kbps
[INFO] Sending broadcast message...
[INFO] TX: ID=0x100, DLC=8, Data=[01 02 03 04 05 06 07 08]
[INFO] RX: ID=0x200, DLC=8, Data=[AA BB CC DD EE FF 00 11]
[INFO] Network status: 3 nodes active
```

## 📊 CAN Message Format

### Standard Frame (11-bit ID)
```
┌─────────┬─────┬─────┬──────────┬─────────┐
│ SOF     │ ID  │ CTL │ DATA     │ CRC     │
│ 1 bit   │ 11b │ 4b  │ 0-8 bytes│ 15 bits │
└─────────┴─────┴─────┴──────────┴─────────┘
```

### Extended Frame (29-bit ID)
```
┌─────────┬──────────┬─────┬─────┬──────────┬─────────┐
│ SOF     │ Base ID  │ SRR │ IDE │ Ext ID   │ ...     │
│ 1 bit   │ 11 bits  │ 1b  │ 1b  │ 18 bits  │ ...     │
└─────────┴──────────┴─────┴─────┴──────────┴─────────┘
```

## ⚙️ Configuration

### CAN Speed Options
```ini
build_flags = 
    -D CAN_SPEED_125K=125000
    -D CAN_SPEED_250K=250000
    -D CAN_SPEED_500K=500000
    -D CAN_SPEED_1M=1000000
```

### Message Queue Settings
```cpp
#define CAN_QUEUE_SIZE    10
#define CAN_TASK_PRIORITY 5
#define CAN_STACK_SIZE    4096
```

## 🧪 Testing

### Loopback Test
```bash
# Enable loopback mode in code
#define CAN_LOOPBACK_MODE true
```

### Static Analysis
```bash
cppcheck --enable=all --std=c++11 src/*.cpp
```

## 🔍 Troubleshooting

### No CAN Messages
1. Check CAN transceiver power (5V vs 3.3V)
2. Verify termination resistors (120Ω at each end)
3. Check CAN speed matches all nodes
4. Verify TX/RX pin connections

### Error Frames
- Check for ground loops
- Verify cable quality (twisted pair)
- Reduce bus length if > 40m
- Check for electromagnetic interference

### Bus Off State
- Implement automatic recovery
- Check for dominant bit errors
- Verify all nodes have proper power

## 📚 References

- [CAN Bus Protocol](https://en.wikipedia.org/wiki/CAN_bus)
- [ESP32 TWAI Controller](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/twai.html)
- [MCP2551 Datasheet](https://ww1.microchip.com/downloads/en/DeviceDoc/21667D.pdf)

## 📄 License

MIT License - see main repository LICENSE file.

---

*Version: 1.0.0*  
*Last Updated: 2025*
