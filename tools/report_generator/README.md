# 📊 Test Report Generator

Tool Python untuk menghasilkan laporan test profesional dari data percobaan embedded systems.

## ✨ Fitur

- **📈 Analisis Statistik Otomatis**: Mean, Min, Max, Standard Deviation
- **✅ Evaluasi Pass/Fail**: Berdasarkan toleransi yang ditentukan
- **📄 Export Multi-Format**:
  - CSV untuk analisis lebih lanjut (Excel, Google Sheets)
  - PDF laporan profesional dengan tabel dan styling
  - PNG grafik multi-plot (RPM, Voltage, Current, Temperature)
- **🔧 Mudah Dikustomisasi**: Tambahkan metric baru sesuai kebutuhan

## 🎯 Use Case untuk Portofolio

Tool ini sangat cocok untuk menunjukkan kompetensi kamu di bidang:
- **Data Analysis & Reporting** - kemampuan mengolah data test
- **Quality Assurance** - evaluasi pass/fail berdasarkan spesifikasi
- **Automation** - generate laporan otomatis dari raw data
- **Professional Documentation** - format laporan standar industri

Perusahaan seperti **Horiba**, **Denso**, **Toyota**, dll sangat menghargai dokumentasi test yang rapi dan terstruktur.

## 📂 Struktur Folder

```
report_generator/
├── test_report_generator.py    # Main script
├── sample_data/
│   └── motor_test_001.json     # Sample data test
├── output/                      # Hasil generate
│   ├── *_report.csv
│   ├── *_report.pdf
│   └── *_plot.png
└── README.md
```

## 🚀 Cara Menggunakan

### 1. Install Dependencies

```bash
pip install pyserial matplotlib reportlab
```

### 2. Pilih Workflow

#### Option A: Capture Data dari ESP32 (Real Hardware)

```bash
# Lihat COM ports tersedia
python esp32_data_logger.py list

# Record data (auto-detect port)
python esp32_data_logger.py record

# Record dengan durasi 60 detik
python esp32_data_logger.py record 60

# Interactive mode
python esp32_data_logger.py
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

### 3. Generate Laporan

**Single file:**
```bash
python test_report_generator.py sample_data/motor_test_001.json
```

**Batch mode (process semua JSON):**
```bash
python test_report_generator.py --batch
```

**Default (gunakan sample data):**
```bash
python test_report_generator.py
```

### 4. Lihat Hasil

Output akan tersimpan di folder `output/`:
- `*_report.csv` - Data + statistik
- `*_report.pdf` - Laporan profesional
- `*_plot.png` - Grafik 4 parameter

## 📊 Contoh Output

### Statistik
```
RPM:     Mean=1500.27, Min=1488, Max=1515, StdDev=8.03
Voltage: Mean=12.05V, Range=11.8-12.3V
Current: Mean=0.45A, Range=0.42-0.48A
Temp:    Mean=26.71°C, Range=25.3-28.2°C
```

### Evaluasi
```
Target: 1500 RPM ±5%
Allowed Range: 1425.0 - 1575.0 RPM
Actual Range: 1488 - 1515 RPM
Result: PASS ✅
```

## 🔧 Kustomisasi

### Menambah Metric Baru

1. Tambahkan field di data JSON:
```json
{"timestamp": "00:00:00", "rpm": 1502, "voltage": 12.1, "new_metric": 42.5}
```

2. Extract di `__init__`:
```python
self.new_metric_values = [dp['new_metric'] for dp in self.data_points]
```

3. Tambahkan statistik di `calculate_statistics()`:
```python
stats['new_metric'] = {
    'mean': mean(self.new_metric_values),
    'min': min(self.new_metric_values),
    'max': max(self.new_metric_values),
    'std_dev': stdev(self.new_metric_values) if len(self.new_metric_values) > 1 else 0
}
```

4. Tambahkan plot di `generate_plots()` jika perlu.

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

### 5. Best Practices
- **Konsistensi Naming**: `{project_name}_test_{version}.json`
- **Document Test Conditions**: Catat environmental conditions, hardware setup
- **Version Control**: Commit JSON files + reports ke Git
- **Regular Testing**: Run tests berkala untuk track degradation

## 📝 License

Free to use for portfolio and educational purposes.

---

**Created for Farisshu Embedded Portfolio** | 2024
