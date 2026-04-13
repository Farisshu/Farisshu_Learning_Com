#include "ui_display.h"

TFT_eSPI tft = TFT_eSPI();

// Layout konstan (anti hardcode di loop)
constexpr uint16_t W = 160;
constexpr uint16_t H = 128;
constexpr uint8_t  MARGIN = 8;
constexpr uint8_t  TITLE_Y = 12;
constexpr uint8_t  TEMP_Y  = 42;
constexpr uint8_t  PRESS_Y = 82;
constexpr uint8_t  STATUS_Y = 114;

void ui_init() {
  tft.init();
  tft.setRotation(1); // Landscape
  tft.fillScreen(TFT_BLACK);
  ui_drawStaticLayout();
}

void ui_drawStaticLayout() {
  tft.setTextColor(TFT_CYAN, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(MARGIN, TITLE_Y);
  tft.print("RS485 MONITORING NODE");

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(1);
  tft.setCursor(MARGIN, TEMP_Y - 14);
  tft.print("TEMPERATURE");

  tft.setCursor(MARGIN, PRESS_Y - 14);
  tft.print("PRESSURE");
}

void ui_update(float temp, float press, bool linkActive) {
  // Clear area nilai saja (efisien)
  tft.fillRect(MARGIN, TEMP_Y, 100, 24, TFT_BLACK);
  tft.fillRect(MARGIN, PRESS_Y, 100, 24, TFT_BLACK);
  tft.fillRect(W - 60, STATUS_Y, 55, 14, TFT_BLACK);

  // Temperature
  tft.setTextColor(TFT_YELLOW, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(MARGIN, TEMP_Y);
  tft.print(temp, 1);
  tft.setTextSize(1);
  tft.print(" C");

  // Pressure
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setTextSize(2);
  tft.setCursor(MARGIN, PRESS_Y);
  tft.print(press, 1);
  tft.setTextSize(1);
  tft.print(" Bar");

  // Link Status
  tft.setTextSize(1);
  if (linkActive) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(W - 55, STATUS_Y);
    tft.print("LINK OK");
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.setCursor(W - 60, STATUS_Y);
    tft.print("OFFLINE");
  }
}