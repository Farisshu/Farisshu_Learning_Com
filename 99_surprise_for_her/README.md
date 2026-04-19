# 99 Surprise For Her

[![Build Status](https://img.shields.io/badge/build-passing-green)]()
[![Code Quality](https://img.shields.io/badge/cppcheck-passing-green)]()
[![FreeRTOS](https://img.shields.io/badge/FreeRTOS-enabled-blue)]()

Special project dengan ESP32 untuk aplikasi creative dan personal.

## 📋 Deskripsi

Project ini adalah implementasi khusus menggunakan ESP32 untuk keperluan personal dengan fitur-fitur interaktif dan visual yang menarik.

## ✨ Fitur

- ✅ **FreeRTOS Multi-tasking** - Concurrent task execution
- ✅ **Interactive UI** - User interface dengan display
- ✅ **Custom Animations** - Visual effects dan animations
- ✅ **Input Handling** - Button/touch input processing
- ✅ **Logging System** - Debug logging support
- ✅ **Low Power Mode** - Power efficient operation

## 🏗️ Struktur Folder

```
99_surprise_for_her/
├── src/
│   ├── main.cpp      # Main application logic
│   └── ...           # Module implementations
├── README.md         # This file
└── platformio.ini    # Build configuration
```

## 🔧 Hardware Requirements

### Supported Boards
- ESP32 DevKit V1
- ESP32-WROOM-32
- NodeMCU-32S

### Optional Peripherals
- Display (OLED/TFT)
- Buttons/Switches
- LEDs (WS2812B/NeoPixel)
- Buzzer/Speaker

## 🚀 Quick Start

### Build & Upload
```bash
cd 99_surprise_for_her
pio run --target upload
pio device monitor
```

### Configuration
Edit `platformio.ini` untuk customize board dan features:
```ini
[env:esp32dev]
board = esp32dev
framework = arduino
build_flags = 
    -D FEATURE_ANIMATION=1
    -D FEATURE_SOUND=1
```

## ⚙️ Customization

### Animation Settings
```cpp
#define ANIMATION_SPEED     100     // ms per frame
#define ANIMATION_LOOPS     5       // number of loops
#define BRIGHTNESS          128     // LED brightness (0-255)
```

### Input Configuration
```cpp
#define BUTTON_PIN          4
#define TOUCH_PIN           2
#define DEBOUNCE_MS         50
```

## 🎨 Features Overview

### Display Output
- Text rendering
- Graphics primitives
- Custom fonts
- Screen transitions

### LED Control
- RGB color mixing
- Pattern animations
- Brightness control
- Power management

### Sound Effects
- Tone generation
- Melody playback
- Volume control

## 🔍 Troubleshooting

### Display Not Working
1. Check I2C/SPI connections
2. Verify display address
3. Initialize display in setup()
4. Check power supply (3.3V vs 5V)

### Buttons Not Responding
- Enable internal pull-up resistors
- Check debouncing implementation
- Verify GPIO pin assignments
- Test with serial monitor

### Memory Issues
- Reduce animation buffer sizes
- Optimize task stack sizes
- Use PROGMEM for constants
- Monitor heap with `ESP.getFreeHeap()`

## 📚 References

- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)
- [FreeRTOS Documentation](https://www.freertos.org/)
- [PlatformIO Docs](https://docs.platformio.org/)

## 📄 License

MIT License - see main repository LICENSE file.

---

*Version: 1.0.0*  
*Last Updated: 2025*  
*Special Project 💝*
