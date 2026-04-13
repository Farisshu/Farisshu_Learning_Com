#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// === LAYAR LANDSCAPE ===
#define W 160
#define H 128
#define CX (W/2)
#define CY (H/2)

// === FRAME CONTROL (30 FPS) ===
#define FPS 30
#define FRAME_MS (1000 / FPS)
unsigned long lastFrame = 0;

// === KONFIGURASI BINTANG ===
#define NUM_STARS 50
struct Star {
  float x, y, z;
  float speed;
};
Star stars[NUM_STARS];

// === LIRIK (Love Me Not - Ravyn Lenae) ===
const char* lyrics[] = {
  "And, oh,",
  "it's hard to see you,",
  "but I wish you were",
  "right here.",
  "", 
  "Oh, it's hard to leave you",
  "when I get you",
  "everywhere."
};
const int TOTAL_LINES = 8;

// === STATE ===
int currentLine = 0;
unsigned long lineStart = 0;
bool finished = false;

// === WARNA ===
#define COL_BG 0x0000      // Hitam
#define COL_TEXT 0xFFFF    // Putih
#define COL_STAR tft.color565(30, 50, 80) // Biru gelap redup

// === INISIALISASI BINTANG ===
void initStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].x = random(-W, W);
    stars[i].y = random(-H, H);
    stars[i].z = random(20, 400); // Kedalaman
    stars[i].speed = random(5, 15) / 10.0; // Kecepatan pelan
  }
}

// === RENDER BINTANG (Background) ===
void drawStars() {
  for (int i = 0; i < NUM_STARS; i++) {
    stars[i].z -= stars[i].speed;
    
    // Reset bintang jika sudah lewat layar
    if (stars[i].z <= 1) {
      stars[i].x = random(-W, W);
      stars[i].y = random(-H, H);
      stars[i].z = 350;
      stars[i].speed = random(5, 15) / 10.0;
      continue;
    }

    // Proyeksi 3D ke 2D
    float sx = (stars[i].x / stars[i].z) * CX + CX;
    float sy = (stars[i].y / stars[i].z) * CY + CY;
    int size = map(stars[i].z, 1, 350, 2, 1);
    
    // Warna berdasarkan jarak (jauh=redup, dekat=terang)
    uint16_t color = map(stars[i].z, 1, 350, COL_TEXT, COL_STAR);
    
    if (sx >= 0 && sx <= W && sy >= 0 && sy <= H) {
      tft.fillCircle(sx, sy, size, color);
    }
  }
}

// === RENDER LIRIK (Foreground) ===
void drawLyrics() {
  unsigned long elapsed = millis() - lineStart;
  
  // Hitung opasitas untuk efek Fade In/Out
  uint8_t alpha = 0;
  
  if (elapsed < 300) {
    // Fade IN (0 - 300ms)
    alpha = map(elapsed, 0, 300, 0, 255);
  } 
  else if (elapsed < 2000) {
    // Fully Visible (300ms - 2000ms)
    alpha = 255;
  } 
  else if (elapsed < 2500) {
    // Fade OUT (2000ms - 2500ms)
    alpha = map(elapsed, 2000, 2500, 255, 0);
  } 
  else {
    // Waktu habis, ganti baris
    currentLine++;
    lineStart = millis();
    alpha = 0;
    
    if (currentLine >= TOTAL_LINES) {
      finished = true;
    }
  }
  
  // Jika belum selesai, gambar teks
  if (currentLine < TOTAL_LINES) {
    // Clear area teks (supaya bintang di belakang tidak tertutup kotak hitam)
    // Kita "lukis ulang" area teks dengan warna background yang sedikit transparan (simulasi)
    // Atau lebih mudah: Clear kotak area teks dengan warna biru tua (supaya teks terbaca jelas di atas bintang)
    tft.fillRect(10, CY - 20, W - 20, 40, tft.color565(0, 0, 15)); 
    
    // Buat warna teks berdasarkan alpha
    uint8_t r = map(alpha, 0, 255, 0, 255);
    uint8_t g = map(alpha, 0, 255, 0, 255);
    uint8_t b = map(alpha, 0, 255, 0, 255);
    uint16_t textColor = tft.color565(r, g, b);

    // Gambar teks
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(textColor, tft.color565(0, 0, 15)); // Background teks biru tua pekat
    tft.setTextSize(1);
    tft.drawString(lyrics[currentLine], CX, CY);
  }
}

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(COL_BG);
  
  initStars();
  lineStart = millis();
  lastFrame = millis();
  
  Serial.println("▶ Starfield Lyric Mode Ready");
}

void loop() {
  if (finished) return;
  
  // === FRAME RATE CONTROL ===
  unsigned long now = millis();
  if (now - lastFrame < FRAME_MS) {
    delay(FRAME_MS - (now - lastFrame));
    return;
  }
  lastFrame = millis();
  
  // === RENDER SPI BATCH (Lebih Cepat & Lancar) ===
  tft.startWrite();
  
  // 1. Gambar Background (Jejak bintang)
  // Menggunakan fillRect semi-transparan untuk efek ekor bintang (motion blur)
  tft.fillRect(0, 0, W, H, tft.color565(0, 0, 5)); 
  
  // 2. Gambar Bintang Baru
  drawStars();
  
  // 3. Gambar Lirik di Atas
  drawLyrics();
  
  // Jika sudah selesai, gambar hati
  if (finished) {
    tft.fillRect(0, 0, W, H, tft.color565(0, 0, 10));
    tft.setTextColor(COL_TEXT, tft.color565(0, 0, 10));
    tft.setTextSize(3);
    tft.drawString("♥", CX, CY);
  }

  tft.endWrite();
}