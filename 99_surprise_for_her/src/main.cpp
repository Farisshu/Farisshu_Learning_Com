#include <Arduino.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// 💌 Pesan untuk dia (bisa kamu edit!)
const char* message[] = {
  "Oh, it's hard to see you,",
  "but I wish you were",
  "right here...",
  "💕 Always, Faris 💕"
};
const int lineCount = 4;

// 🎨 Konfigurasi Animasi
#define HEART_X 60
#define HEART_Y 15
#define TEXT_START_Y 55
#define TYPE_SPEED 80      // Kecepatan mengetik (ms per char)
#define HEART_BEAT_MS 400  // Kecepatan denyut hati

// State
int currentLine = 0;
int charIndex = 0;
unsigned long lastCharTime = 0;
bool heartExpanded = false;
unsigned long lastHeartTime = 0;
bool animationDone = false;

// ❤️ Fungsi gambar hati (pixel art 11x10)
void drawHeart(int x, int y, uint16_t color, bool expanded) {
  int scale = expanded ? 1 : 0;
  // Simple heart shape using drawPixel
  for (int dy = 0; dy < 10 + scale*2; dy++) {
    for (int dx = 0; dx < 11 + scale*2; dx++) {
      // Pola hati sederhana (bisa diganti dengan bitmap jika mau lebih detail)
      if ((dx >= 2-scale && dx <= 8+scale && dy >= 0 && dy <= 3+scale) ||
          (dx >= 0-scale && dx <= 10+scale && dy >= 2+scale && dy <= 5+scale) ||
          (dx >= 1+scale && dx <= 9+scale && dy >= 4+scale && dy <= 9+scale*2)) {
        tft.drawPixel(x + dx, y + dy, color);
      }
    }
  }
}

// 🎨 Gradient background romantis
void drawRomanticBackground() {
  for (int y = 0; y < 128; y++) {
    // Gradient: Pink → Purple → Dark Blue
    uint8_t r = map(y, 0, 128, 255, 30);
    uint8_t g = map(y, 0, 128, 180, 50);
    uint8_t b = map(y, 0, 128, 200, 100);
    tft.drawFastHLine(0, y, 160, tft.color565(r, g, b));
  }
}

// ✍️ Efek mengetik per karakter
void typeLine(const char* text, int lineNum) {
  if (charIndex < strlen(text)) {
    if (millis() - lastCharTime >= TYPE_SPEED) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.setTextSize(1);
      tft.setCursor(10, TEXT_START_Y + lineNum * 18);
      
      // Gambar teks sampai karakter ke charIndex
      char buffer[32];
      strncpy(buffer, text, charIndex + 1);
      buffer[charIndex + 1] = '\0';
      tft.print(buffer);
      
      charIndex++;
      lastCharTime = millis();
    }
  } else {
    // Lanjut ke baris berikutnya setelah jeda
    if (millis() - lastCharTime >= 800) {
      currentLine++;
      charIndex = 0;
      lastCharTime = millis();
      if (currentLine >= lineCount) {
        animationDone = true;
      }
    }
  }
}

void setup() {
  Serial.begin(115200);
  
  tft.init();
  tft.setRotation(1); // Landscape
  
  drawRomanticBackground();
  
  // ❤️ Gambar hati awal
  drawHeart(HEART_X, HEART_Y, TFT_RED, false);
  
  Serial.println("💌 Love letter animation started");
  lastCharTime = millis();
  lastHeartTime = millis();
}

void loop() {
  // 💓 Denyut hati (selalu jalan)
  if (millis() - lastHeartTime >= HEART_BEAT_MS) {
    // Clear area hati dulu (gambar kotak hitam kecil)
    tft.fillRect(HEART_X-1, HEART_Y-1, 14, 12, tft.color565(30, 50, 100));
    heartExpanded = !heartExpanded;
    drawHeart(HEART_X, HEART_Y, TFT_RED, heartExpanded);
    lastHeartTime = millis();
  }
  
  // ✍️ Animasi mengetik (hanya jika belum selesai)
  if (!animationDone && currentLine < lineCount) {
    typeLine(message[currentLine], currentLine);
  }
  
  // 🔄 Jika selesai, bisa loop ulang atau tampilkan efek akhir
  if (animationDone) {
    // Opsional: Tambahkan efek bintang/konfeti kecil di sekitar teks
    // Atau biarkan statis dengan hati tetap berdenyut
  }
  
  delay(10); // Small delay untuk stabilitas
}