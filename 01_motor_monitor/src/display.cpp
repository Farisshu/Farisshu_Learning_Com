#include "display.h"

TFT_eSPI tft = TFT_eSPI();

void initDisplay() {
  tft.init();
  tft.setRotation(1);  // Landscape 160x128
  tft.fillScreen(TFT_BLACK);
  
  // Welcome screen
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(20, 30);
  tft.print("MOTOR");
  tft.setCursor(10, 60);
  tft.print("MONITOR");
  delay(1000);
}

void clearScreenWithGradient() {
  // Full screen gradient (160x128)
  for (int y = 0; y < 128; y++) {
    uint16_t color = tft.color565(0, 40 + y, 80 + y/2);
    tft.drawFastHLine(0, y, 160, color);
  }
}

void drawMotorDisplay(float rpm) {
  clearScreenWithGradient();
  
  // Judul (atas kiri)
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(5, 5);
  tft.print("MOTOR RPM");
  
  // RPM Value (BESAR - tengah)
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(4);
  tft.setCursor(25, 35);
  tft.print((int)rpm);
  
  // Label RPM
  tft.setTextSize(2);
  tft.setCursor(120, 50);
  tft.print("RPM");
  
  // Progress bar background (lebar penuh)
  tft.fillRect(5, 80, 150, 25, TFT_DARKGREY);
  
  // Progress bar fill
  int fillWidth = map(constrain(rpm, 0, 3000), 0, 3000, 0, 150);
  uint16_t barColor;
  if (rpm < 1000) {
    barColor = TFT_GREEN;
  } else if (rpm < 2000) {
    barColor = TFT_YELLOW;
  } else {
    barColor = TFT_RED;
  }
  tft.fillRect(6, 81, fillWidth, 23, barColor);
  
  // Border
  tft.drawRect(5, 80, 150, 25, TFT_WHITE);
  
  // Scale labels
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(5, 110);
  tft.print("0");
  
  tft.setCursor(65, 110);
  tft.print("1500");
  
  tft.setCursor(130, 110);
  tft.print("3000");
  
  // Status indicator
  tft.setCursor(5, 118);
  if (rpm < 500) {
    tft.setTextColor(TFT_BLUE, TFT_BLACK);
    tft.print("IDLE");
  } else if (rpm < 2000) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.print("RUNNING");
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.print("HIGH");
  }
}