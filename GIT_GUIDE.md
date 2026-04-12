## 📖 Panduan Lengkap: Git & PlatformIO untuk Embedded

**Repo**: `Farisshu_Learning_Com`  
**Untuk**: Persiapan Magang Embedded System Engineer  
**Author**: M Faris Asroru Ghifary

---

### 📋 DAFTAR ISI
1. [Setup Awal](#1-setup-awal)
2. [Workflow Harian](#2-workflow-harian)
3. [Perintah Git Penting](#3-perintah-git-penting)
4. [Troubleshooting](#4-troubleshooting)
5. [Struktur Proyek](#5-struktur-proyek)
6. [PlatformIO Tips](#6-platformio-tips)

---

## 1. SETUP AWAL

### 🔧 Install & Konfigurasi
```powershell
# 1. Buka terminal di folder proyek
cd "C:\path\ke\proyek"

# 2. Inisialisasi Git
git init

# 3. Set identitas (PENTING: sesuaikan dengan akun GitHub)
git config user.name "Farisshu"
git config user.email "m.farisasrorughifary@gmail.com"

# 4. Buat .gitignore (WAJIB)
# Isi .gitignore:
.pio/
.vscode/
*.log
.DS_Store

# 5. Link ke GitHub
git remote add origin https://github.com/Farisshu/Farisshu_Learning_Com.git

# 6. Commit pertama
git add .
git commit -m "Initial commit"

# 7. Push ke GitHub
git push -u origin main
```

### 🔐 Setup Credential (Jika Error 403)
1. Buka **Credential Manager** di Windows
2. Hapus `git:https://github.com` yang lama
3. Push ulang → login via browser dengan akun GitHub

---

## 2. WORKFLOW HARIAN

### ✅ Sebelum Mulai Coding
```powershell
# 1. Cek identitas Git (PENTING jika komputer dipakai bergantian!)
git config user.name
git config user.email

# 2. Ambil update terbaru (jika coding di banyak PC)
git pull origin main
```

### 💻 Setelah Coding/Ubah File
**Cara 1: Pakai GUI (VS Code)**
1. Buka panel **Source Control** (`Ctrl+Shift+G`)
2. Tulis pesan commit
3. Klik `✓` (Commit)
4. Klik `...` → **Sync** (Push + Pull sekaligus)

**Cara 2: Pakai Terminal**
```powershell
# 1. Cek file yang berubah
git status

# 2. Tambah semua file
git add .

# 3. Commit dengan pesan jelas
git commit -m "Feat: tambah fungsi alarm"

# 4. Push ke GitHub
git push
# atau
git push origin main
```

### 📥 Di Komputer Lain (Sync)
```powershell
# 1. Clone (pertama kali)
git clone https://github.com/Farisshu/Farisshu_Learning_Com.git

# 2. Pull (update selanjutnya)
cd Farisshu_Learning_Com
git pull origin main
```

---

## 3. PERINTAH GIT PENTING

### 📊 Monitoring
```powershell
git status          # Cek file berubah
git log --oneline   # Lihat riwayat commit (1 baris)
git diff            # Lihat perubahan detail
git branch          # Lihat branch aktif
git remote -v       # Cek remote repository
```

### 🔧 Perbaikan
```powershell
# Undo perubahan file (belum commit)
git checkout -- nama_file.txt

# Undo commit terakhir (masih simpan perubahan)
git reset HEAD~1

# Ganti pesan commit terakhir
git commit --amend -m "Pesan yang benar"

# Paksa push (HATI-HATI: overwrite remote)
git push --force-with-lease
```

### 🌿 Branch (Opsional)
```powershell
# Buat branch baru
git checkout -b fitur-baru

# Pindah branch
git checkout main

# Gabungkan branch
git merge fitur-baru

# Push branch baru
git push -u origin fitur-baru
```

---

## 4. TROUBLESHOOTING

### ❌ Error: "Permission denied (403)"
**Penyebab**: Git login dengan akun salah  
**Solusi**:
```powershell
# 1. Hapus credential lama di Credential Manager
# 2. Update remote URL
git remote set-url origin https://github.com/Farisshu/Farisshu_Learning_Com.git
# 3. Push ulang → login via browser
git push
```

### ❌ Error: "nothing to commit, working tree clean"
**Penyebab**: Tidak ada file yang dilacak  
**Solusi**:
```powershell
# Cek isi .gitignore - jangan sampai ada "*"
notepad .gitignore

# Paksa add file
git add -f README.md
git commit -m "Add README"
```

### ❌ Error: "src refspec main does not match any"
**Penyebab**: Belum ada commit atau branch salah  
**Solusi**:
```powershell
# Cek branch
git branch

# Kalau masih 'master', push ke master dulu
git push -u origin master

# Atau ganti ke main
git branch -m master main
git push -u origin main
```

### ❌ File tidak muncul di GitHub
**Penyebab**: File di-ignore atau folder kosong  
**Solusi**:
```powershell
# Untuk folder kosong, buat file .gitkeep
New-Item folder_kosong/.gitkeep
git add .
git commit -m "Add gitkeep"
git push
```

### ❌ Konflik Merge
**Penyebab**: File sama diedit di 2 tempat  
**Solusi**:
1. Buka file yang konflik (ada tanda `<<<<<<<`)
2. Pilih kode yang mau dipakai
3. Hapus tanda konflik
4. Save → `git add .` → `git commit` → `git push`

---

## 5. STRUKTUR PROYEK

### 📁 Template Standar
```
nama_proyek/
├── 01_motor_monitor/      # Proyek 1
│   ├── platformio.ini     # Config PlatformIO
│   ├── src/
│   │   └── main.cpp       # Kode utama
│   └── README.md          # Doc spesifik proyek
│
├── 02_waveform_graph/     # Proyek 2
│   ├── platformio.ini
│   └── src/
│
├── 03_industrial_hmi/     # Proyek 3
│   ├── platformio.ini
│   └── src/
│
├── .gitignore             # File yang di-ignore
├── README.md              # Dokumentasi utama
└── GIT_GUIDE.md           # Panduan ini
```

### 📝 .gitignore (WAJIB)
```gitignore
# Build output
.pio/
.piolibdeps/

# VS Code
.vscode/

# Log files
*.log
*.tmp

# OS
.DS_Store
Thumbs.db
```

### 📖 README.md Template
```markdown
# Nama Proyek
Deskripsi singkat (1-2 kalimat)

## 📋 Fitur
- Fitur 1
- Fitur 2

## 🔌 Wiring
| Pin | ESP32 |
|-----|-------|
| VCC | 3V3   |
| GND | GND   |

## 🚀 Cara Upload
1. Buka folder di VS Code
2. `pio run -t upload`
3. Serial Monitor 115200 baud
```

---

## 6. PLATFORMIO TIPS

### ⚡ Command Penting
```powershell
# Build & upload
pio run -t upload

# Build saja
pio run

# Clean build
pio run -t clean

# Serial monitor
pio device monitor
# atau
pio device monitor --baud 115200

# List device
pio device list

# Install library
pio lib install "TFT_eSPI"

# Update library
pio lib update
```

### 🔧 Config platformio.ini
```ini
[env:esp32dev]
platform = espressif32
board = esp32dev
framework = arduino
monitor_speed = 115200
lib_deps = bodmer/TFT_eSPI@^2.5.43

# Build flags (config hardware)
build_flags = 
  -D USER_SETUP_LOADED=1
  -D ST7735_DRIVER=1
  -D TFT_WIDTH=128
  -D TFT_HEIGHT=160
  -D TFT_MOSI=23
  -D TFT_SCLK=18
  -D TFT_CS=5
  -D TFT_DC=17
  -D TFT_RST=4
```

### 🐛 Debugging
```cpp
// Serial debug
Serial.begin(115200);
Serial.println("Debug: " + String(nilai));

// LED indicator
digitalWrite(LED_BUILTIN, HIGH); // Nyala
delay(100);
digitalWrite(LED_BUILTIN, LOW);  // Mati
```

---

## 📚 CHECKLIST SEBELUM PUSH

- [ ] `git config user.email` sudah benar?
- [ ] File sensitif (password, API key) tidak ter-commit?
- [ ] `.gitignore` sudah berisi `.pio/`?
- [ ] Pesan commit jelas & deskriptif?
- [ ] Sudah build & test di hardware?
- [ ] README updated jika ada fitur baru?

---

## 🎯 BEST PRACTICE

### ✅ Commit Message Format
```
Jenis: Deskripsi singkat

Jenis:
- Feat: Fitur baru
- Fix: Perbaikan bug
- Docs: Update dokumentasi
- Style: Format code (spasi, indent)
- Refactor: Refactor code
- Test: Tambah test
- Chore: Config, dependency
```

**Contoh:**
```
Feat: tambah alarm flash di status bar
Fix: crash di updateDisplay saat RPM 0
Docs: update wiring diagram di README
```

### 📌 Aturan Emas
1. **Commit kecil & sering** → Lebih mudah track perubahan
2. **Jangan commit file build** (`.pio`, `.o`, `.elf`)
3. **Test dulu sebelum push** → Pastikan tidak break
4. **Pull sebelum push** → Hindari konflik
5. **Pesan commit yang jelas** → Bayangkan orang lain baca

---

## 🔗 REFERENSI

- [Git Documentation](https://git-scm.com/doc)
- [PlatformIO Docs](https://docs.platformio.org/)
- [GitHub Guides](https://guides.github.com/)
- [Pro Git Book](https://git-scm.com/book/en/v2)

---

**Last Updated**: April 2026  
**Author**: M Faris Asroru Ghifary  
**Contact**: m.farisasrorughifary@gmail.com

---

## Cara Pakai File Ini:

1. **Copy semua teks di atas**
2. **Buat file baru** di repo: `GIT_GUIDE.md`
3. **Paste & Save**
4. **Commit & Push**:
   ```powershell
   git add GIT_GUIDE.md
   git commit -m "Docs: add comprehensive Git guide"
   git push
   ```
