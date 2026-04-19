# 🚀 Quick Start Guide - Report Generator

## 3 Menit Setup & Test

### 1. Install Dependencies
```bash
pip install pyserial matplotlib reportlab
```

### 2. Generate Sample Data
```bash
cd tools/report_generator
python esp32_data_logger.py simulate 15
```

### 3. Generate Report
```bash
python test_report_generator.py --batch
```

### 4. Lihat Hasil
```bash
# Windows
explorer output

# Linux
xdg-open output

# Mac
open output
```

---

## Commands Cheat Sheet

| Command | Deskripsi |
|---------|-----------|
| `python esp32_data_logger.py list` | List COM ports tersedia |
| `python esp32_data_logger.py simulate 15` | Generate 15 data simulasi |
| `python esp32_data_logger.py record` | Record dari ESP32 (30 detik) |
| `python esp32_data_logger.py record 60` | Record dari ESP32 (60 detik) |
| `python test_report_generator.py` | Generate report (default sample) |
| `python test_report_generator.py file.json` | Generate dari file spesifik |
| `python test_report_generator.py --batch` | Process semua JSON sekaligus |

---

## Output Files

Setiap test menghasilkan 3 files:

1. **`*_report.csv`** - Data + statistik (buka di Excel)
2. **`*_report.pdf`** - Laporan profesional (untuk portfolio)
3. **`*_plot.png`** - Grafik 4 parameter (visualisasi)

---

## Next Steps

- 👉 Baca [`README.md`](README.md) untuk detail fitur
- 👉 Baca [`README_WORKFLOW.md`](README_WORKFLOW.md) untuk workflow lengkap
- 🎯 **Goal**: Generate reports untuk semua 9 project ESP32 kamu!

**Good luck! 🚀**
