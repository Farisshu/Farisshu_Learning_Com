# ESP32 FreeRTOS Projects Collection

[![CI/CD](https://github.com/farishu/esp32-freertos-projects/actions/workflows/ci.yml/badge.svg)](https://github.com/farishu/esp32-freertos-projects/actions/workflows/ci.yml)
[![Code Quality](https://img.shields.io/badge/code%20quality-cppcheck-green)](https://cppcheck.sourceforge.io/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-6.1-orange)](https://platformio.org/)

Kumpulan project ESP32 berbasis FreeRTOS dengan standar industri untuk embedded systems development.

## 📋 Daftar Project

| No | Project | Status | Deskripsi | Build |
|----|---------|--------|-----------|-------|
| 1 | [01_motor_monitor](01_motor_monitor/) | ✅ Ready | Monitoring motor dengan simulasi sensor dan display | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 2 | [02_waveform_graph](02_waveform_graph/) | ✅ Ready | Visualisasi waveform grafik real-time | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 3 | [03_industrial_hmi](03_industrial_hmi/) | ✅ Ready | HMI (Human Machine Interface) untuk industri | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 4 | [05_rs485_master](05_rs485_master/) | ✅ Ready | RS485 communication master node | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 5 | [06_rs485_slave](06_rs485_slave/) | ✅ Ready | RS485 communication slave node | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 6 | [07_can_bus_node/master](07_can_bus_node/master/) | ✅ Ready | CAN Bus master node | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 7 | [07_can_bus_node/slave](07_can_bus_node/slave/) | ✅ Ready | CAN Bus slave node | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 8 | [990_ESP32_SPI_MASTER_SLAVE](990_ESP32_SPI_MASTER_SLAVE/) | ✅ Ready | SPI communication master-slave | [![Build](https://img.shields.io/badge/build-passing-green)]() |
| 9 | [99_surprise_for_her](99_surprise_for_her/) | ✅ Ready | Special project | [![Build](https://img.shields.io/badge/build-passing-green)]() |

## 🏗️ Struktur Folder Standar

Setiap project mengikuti struktur standar PlatformIO:

```
project_name/
├── include/          # Header files publik
├── lib/              # Library eksternal/custom
├── src/
│   ├── main.cpp      # Entry point (setup & loop)
│   ├── *.h           # Module headers
│   └── *.cpp         # Module implementations
├── test/             # Unit tests (Unity framework)
├── README.md         # Dokumentasi project
└── platformio.ini    # Konfigurasi build
```

## ✨ Fitur Utama

### Standarisasi Kode
- ✅ **Doxygen Comments** - Dokumentasi lengkap untuk semua fungsi dan variabel
- ✅ **Header Guards** - Format konsisten `#ifndef _FILENAME_H_`
- ✅ **Naming Convention**:
  - `camelCase` untuk functions dan variables
  - `PascalCase` untuk types, structs, dan enums
  - `UPPER_CASE` untuk constants dan macros
- ✅ **Static Encapsulation** - Private variables/functions dengan `static`
- ✅ **Error Handling** - NULL checks, bounds checking, error logging
- ✅ **FreeRTOS Compliance** - Task priorities, stack sizes, queue handling

### Configuration Global
File `/include/config.h` menyediakan:
- Logging macros (`LOG_ERROR`, `LOG_WARN`, `LOG_INFO`, `LOG_DEBUG`)
- Error handling macros (`CHECK_NULL`, `CHECK_ESP_ERR`)
- FreeRTOS utilities (`TASK_DELAY_MS`, `TASK_DELAY_UNTIL`)
- Stack size definitions (`STACK_SMALL`, `STACK_MEDIUM`, `STACK_LARGE`)
- Task priorities (`PRIORITY_LOW`, `PRIORITY_NORMAL`, `PRIORITY_HIGH`)

### CI/CD Pipeline
- ✅ Automated build testing untuk semua projects
- ✅ Static analysis dengan cppcheck
- ✅ Code quality checks
- ✅ Build artifacts upload

## 🚀 Quick Start

### Prerequisites
- PlatformIO Core 6.0+
- ESP32 board support package
- Python 3.8+

### Installation
```bash
# Install PlatformIO
pip install platformio

# Clone repository
git clone https://github.com/farishu/esp32-freertos-projects.git
cd esp32-freertos-projects

# Build specific project
cd 01_motor_monitor
pio run

# Upload to device
pio run --target upload
```

### Run Tests
```bash
# Run unit tests
pio test

# Static analysis
cppcheck --enable=all --std=c++11 src/*.cpp
```

## 🛠️ Development Workflow

### 1. Create New Project
```bash
pio init --board esp32dev --project-name my_project
```

### 2. Add Dependencies
Edit `platformio.ini`:
```ini
lib_deps =
    bblanchon/ArduinoJson@^6.21.3
    lvgl/lvgl@^8.3.0
```

### 3. Build & Upload
```bash
pio run --target upload
pio device monitor
```

## 📊 Test & Reporting Tools

### Report Generator (tools/report_generator/)

Tool profesional untuk **capture data dari ESP32** → **generate laporan otomatis** (CSV, PDF, Grafik).

**Fitur:**
- ✅ **Data Analysis**: Statistik otomatis (Mean, Min, Max, Std Dev)
- ✅ **Quality Assurance**: Evaluasi Pass/Fail berdasarkan toleransi
- ✅ **Multi-format Export**: CSV, PDF, PNG (grafik multi-plot)
- ✅ **Automation**: Batch processing untuk multiple tests
- ✅ **Professional Documentation**: Laporan standar industri

**Quick Start:**
```bash
cd tools/report_generator

# Install dependencies
pip install pyserial matplotlib reportlab

# Option A: Simulasi data (testing/demo)
python esp32_data_logger.py simulate 15

# Option B: Capture dari ESP32 real
python esp32_data_logger.py record 60

# Generate laporan (single atau batch)
python test_report_generator.py --batch
```

**Output:**
- `*_report.csv` - Data + statistik
- `*_report.pdf` - Laporan profesional  
- `*_plot.png` - Grafik parameter

**Perfect untuk portofolio Horiba!** Menunjukkan kemampuan:
- Data analysis & statistics
- Professional documentation
- Automation mindset
- Quality assurance thinking

👉 Lihat [`tools/report_generator/README.md`](tools/report_generator/README.md) untuk detail lengkap.

## 📊 Code Quality Metrics

| Metric | Target | Status |
|--------|--------|--------|
| cppcheck errors | 0 | ✅ Pass |
| cppcheck warnings | < 10 | ✅ Pass |
| Code coverage | > 80% | ⏳ Pending |
| Build success rate | 100% | ✅ Pass |

## 🔧 Troubleshooting

### Common Issues

#### Build Failed
```bash
# Clean build environment
pio run --target clean
pio run
```

#### Serial Monitor Not Working
```bash
# Check port permissions
sudo usermod -a -G dialout $USER
# Reboot or logout/login
```

#### Memory Issues
- Check stack sizes in `config.h`
- Reduce task priorities if heap exhaustion occurs
- Use `xPortGetFreeHeapSize()` for monitoring

## 📚 Resources

- [ESP32 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32_technical_reference_manual_en.pdf)
- [FreeRTOS Documentation](https://www.freertos.org/Documentation/api.html)
- [PlatformIO Documentation](https://docs.platformio.org/)
- [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)

## 🤝 Contributing

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 👨‍💻 Author

**Farisshu Embedded Project**

---

*Last Updated: 2025*
*Total Projects: 9*
*All Projects Standardized: ✅*
