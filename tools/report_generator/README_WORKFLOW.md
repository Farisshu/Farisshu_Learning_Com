# 📊 ESP32 Test Data Pipeline - Complete Workflow

## Overview

Pipeline lengkap untuk **capture data dari ESP32** → **simpan ke JSON** → **generate laporan profesional** (CSV, PDF, Grafik).

Tool ini dirancang untuk menunjukkan kompetensi kamu di bidang:
- ✅ **Data Analysis** - Statistik, evaluasi pass/fail
- ✅ **Documentation** - Laporan profesional standar industri
- ✅ **Automation** - Batch processing, workflow terstruktur
- ✅ **Modern Engineering Practices** - Tooling yang efisien dan repeatable

---

## 🛠️ Tools yang Tersedia

### 1. `esp32_data_logger.py` - Data Capture
Capture data real-time dari ESP32 Serial Monitor dan save ke JSON.

**Fitur:**
- Auto-detect COM port
- Support multiple data format parsing
- Real-time monitoring dengan live display
- Simulasi data untuk testing tanpa hardware
- Interactive mode & command-line mode

### 2. `test_report_generator.py` - Report Generation
Generate laporan profesional dari data JSON.

**Fitur:**
- Analisis statistik otomatis (Mean, Min, Max, Std Dev)
- Evaluasi Pass/Fail berdasarkan toleransi
- Export multi-format: CSV, PDF, PNG (grafik)
- Single file mode & batch mode

---

## 📋 Workflow Lengkap

```
┌─────────────────┐      ┌──────────────────┐      ┌──────────────────┐
│   ESP32 Device  │─────▶│  esp32_data_     │─────▶│  test_report_    │
│   (Serial Out)  │      │  logger.py       │      │  generator.py    │
└─────────────────┘      │  (Capture to     │      │  (Generate       │
                         │   JSON)          │      │   Reports)       │
                         └──────────────────┘      └──────────────────┘
                                                         │
                                                         ▼
                                          ┌──────────────────────────┐
                                          │  Output:                 │
                                          │  - CSV (data + stats)    │
                                          │  - PDF (professional)    │
                                          │  - PNG (multi-plot)      │
                                          └──────────────────────────┘
```

---

## 🚀 Cara Menggunakan

### Step 1: Install Dependencies

```bash
pip install pyserial matplotlib reportlab
```

### Step 2: Capture Data dari ESP32

#### Option A: Gunakan Hardware ESP32 (Real Data)

1. **Upload code ke ESP32** dengan output serial format:
   ```cpp
   // Contoh format output
   Serial.printf("%02d:%02d:%02d,%d,%.2f,%.2f,%.1f\n", 
                 hour, minute, second, 
                 rpm, voltage, current, temperature);
   // Output: 14:30:45,1500,12.10,0.45,25.3
   ```

2. **Run data logger:**
   ```bash
   # List available ports
   python esp32_data_logger.py list
   
   # Record dengan auto-detect
   python esp32_data_logger.py record
   
   # Record dengan durasi 60 detik
   python esp32_data_logger.py record 60
   
   # Record dengan port spesifik
   python esp32_data_logger.py record 60 COM3
   ```

3. **Interactive mode:**
   ```bash
   python esp32_data_logger.py
   # Pilih menu [2] Record from ESP32
   ```

#### Option B: Simulasi Data (Testing/Demo)

```bash
# Generate 15 data points simulasi
python esp32_data_logger.py simulate 15

# Interactive mode
python esp32_data_logger.py
# Pilih menu [1] Simulate data
```

Output: `sample_data/simulated_test_YYYYMMDD_HHMMSS.json`

---

### Step 3: Generate Laporan

#### Option A: Single File Mode

```bash
# Generate dari sample data default
python test_report_generator.py

# Generate dari file spesifik
python test_report_generator.py sample_data/motor_test_001.json

# Generate dari file custom
python test_report_generator.py path/to/your_test.json
```

#### Option B: Batch Mode (Process Semua File)

```bash
# Process semua JSON di folder sample_data/
python test_report_generator.py --batch
```

Output akan tersimpan di `output/`:
- `*_report.csv` - Data + statistik
- `*_report.pdf` - Laporan profesional
- `*_plot.png` - Grafik 4 parameter

---

## 📁 Struktur Folder

```
report_generator/
├── esp32_data_logger.py        # Tool capture data dari ESP32
├── test_report_generator.py    # Tool generate laporan
├── README_WORKFLOW.md          # Dokumentasi ini
├── sample_data/                # Input JSON files
│   ├── motor_test_001.json
│   └── simulated_test_*.json
├── output/                     # Generated reports
│   ├── *_report.csv
│   ├── *_report.pdf
│   └── *_plot.png
└── README.md                   # Quick reference
```

---

## 📊 Format Data JSON

```json
{
  "test_name": "Motor RPM Stability Test",
  "device_id": "ESP32_001",
  "operator": "Farisshu",
  "date": "2024-04-19",
  "duration_minutes": 30,
  "target_rpm": 1500,
  "tolerance_percent": 5,
  "data_points": [
    {"timestamp": "00:00:00", "rpm": 1502, "voltage": 12.1, "current": 0.45, "temperature": 25.3},
    {"timestamp": "00:01:00", "rpm": 1498, "voltage": 12.0, "current": 0.46, "temperature": 25.5}
  ]
}
```

### Supported Serial Formats

Tool dapat parse berbagai format output dari ESP32:

**Format 1 - CSV dengan timestamp:**
```
14:30:45,1500,12.10,0.45,25.3
```

**Format 2 - Key-Value:**
```
RPM:1500,VOLT:12.1,CURR:0.45,TEMP:25.3
```

**Format 3 - Simple CSV:**
```
1500,12.10,0.45,25.3
```

---

## 🎯 Contoh Use Case untuk Portofolio

### Project 1: Motor Monitoring (01_motor_monitor)

```bash
# 1. Simulate data
python esp32_data_logger.py simulate 30

# 2. Rename file
mv sample_data/simulated_test_*.json sample_data/motor_monitor_test.json

# 3. Generate report
python test_report_generator.py sample_data/motor_monitor_test.json

# 4. Lihat hasil di output/
```

### Project 2: Multiple Test Comparison

```bash
# 1. Generate multiple test scenarios
python esp32_data_logger.py simulate 20  # Test 1: Normal
python esp32_data_logger.py simulate 20  # Test 2: High load
python esp32_data_logger.py simulate 20  # Test 3: Stress test

# 2. Batch generate all reports
python test_report_generator.py --batch

# 3. Compare results in output/
```

---

## 💡 Tips untuk Portofolio Horiba

### 1. Highlight Data Analysis Skills
- Tunjukkan pemahaman statistik (mean, std dev, tolerance)
- Jelaskan kriteria pass/fail yang digunakan
- Bandingkan multiple test conditions

### 2. Professional Documentation
- Sertakan PDF report di portfolio
- Jelaskan setup test (hardware, kondisi, durasi)
- Visualisasikan data dengan grafik

### 3. Show Automation Mindset
- Demonstrasikan batch processing capability
- Jelaskan workflow yang efficient dan repeatable
- Tunjukkan integrasi antara tools

### 4. Quality Assurance Thinking
- Definisikan acceptance criteria yang jelas
- Documentasikan test procedures
- Track variations dan anomalies

---

## 🔧 Kustomisasi

### Menambah Metric Baru

1. **Update JSON format:**
```json
{"timestamp": "00:00:00", "rpm": 1500, "voltage": 12.1, "new_metric": 42.5}
```

2. **Update `esp32_data_logger.py`:**
   - Tambahkan parsing untuk metric baru
   - Update `parse_serial_line()` method

3. **Update `test_report_generator.py`:**
   - Extract metric di `__init__()`
   - Tambahkan statistik di `calculate_statistics()`
   - Tambahkan plot di `generate_plots()` jika perlu

### Modifikasi Toleransi Criteria

Edit di file JSON:
```json
{
  "target_rpm": 1500,
  "tolerance_percent": 5  // Change to 10 for ±10%
}
```

---

## 📝 Best Practices

1. **Konsistensi Naming:**
   - Gunakan format: `{project_name}_test_{version}.json`
   - Example: `motor_monitor_test_001.json`

2. **Document Test Conditions:**
   - Catat environmental conditions (suhu ruangan, humidity)
   - Document hardware setup (power supply, sensor type)
   - Note any anomalies during test

3. **Version Control:**
   - Commit JSON files ke Git
   - Tag releases untuk major test campaigns
   - Keep raw data + generated reports together

4. **Regular Testing:**
   - Run tests secara berkala untuk track degradation
   - Compare results over time
   - Identify trends and patterns

---

## 🎓 Learning Outcomes

Dengan menggunakan tool ini, kamu menunjukkan:

✅ **Technical Skills:**
- Python programming
- Serial communication
- Data analysis & statistics
- PDF/CSV generation
- Data visualization

✅ **Engineering Mindset:**
- Systematic testing approach
- Quality assurance thinking
- Documentation discipline
- Automation & efficiency

✅ **Professional Competencies:**
- Standard reporting format
- Clear communication of results
- Data-driven decision making
- Modern tooling usage

---

## 📞 Troubleshooting

### Problem: "No COM port found"
**Solution:**
- Pastikan ESP32 terhubung via USB
- Install driver CH340/CP210x jika perlu
- Check Device Manager (Windows) atau `ls /dev/tty*` (Linux/Mac)

### Problem: "No data received"
**Solution:**
- Verify baudrate match (default 115200)
- Check ESP32 is actually sending data
- Try different COM port

### Problem: "Report generation failed"
**Solution:**
- Verify JSON format is correct
- Check dependencies installed (`pip install matplotlib reportlab`)
- Look at error message for details

---

## 📄 License

Free to use for portfolio and educational purposes.

**Created for Farisshu Embedded Portfolio** | 2024

---

## 🙏 Next Steps

1. **Test dengan data real dari ESP32 project kamu**
2. **Generate reports untuk setiap project** (01_motor_monitor, 02_waveform_graph, dll)
3. **Compile semua PDF reports** menjadi portfolio document
4. **Share di LinkedIn/GitHub** sebagai showcase kemampuanmu!

Good luck dengan magang di Horiba! 🚀
