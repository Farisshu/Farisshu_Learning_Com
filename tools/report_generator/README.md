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
pip install matplotlib reportlab
```

### 2. Siapkan Data Test (JSON)

Format JSON:

```json
{
  "test_name": "Motor RPM Stability Test",
  "device_id": "ESP32_001",
  "operator": "Farisshu",
  "date": "2024-04-19",
  "target_rpm": 1500,
  "tolerance_percent": 5,
  "data_points": [
    {"timestamp": "00:00:00", "rpm": 1502, "voltage": 12.1, "current": 0.45, "temperature": 25.3},
    {"timestamp": "00:01:00", "rpm": 1498, "voltage": 12.0, "current": 0.46, "temperature": 25.5}
  ]
}
```

### 3. Generate Laporan

**Gunakan sample data:**
```bash
python test_report_generator.py
```

**Gunakan file custom:**
```bash
python test_report_generator.py path/to/your_test.json
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

## 💡 Tips untuk Portofolio

1. **Dokumentasikan Proses Test**: Jelaskan setup hardware, kondisi lingkungan, durasi test
2. **Visualisasikan Data**: Grafik membantu recruiter memahami hasil test dengan cepat
3. **Highlight Pass/Fail Criteria**: Tunjukkan bahwa kamu paham spesifikasi teknis
4. **Bandingkan Varian**: Test dengan parameter berbeda (misal: RPM 1000 vs 2000)
5. **Integrasikan dengan Proyek Lain**: Gunakan tool ini untuk project ESP32 kamu

## 📝 License

Free to use for portfolio and educational purposes.

---

**Created for Farisshu Embedded Portfolio** | 2024
