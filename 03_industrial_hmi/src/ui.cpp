#include "ui.h"

TFT_eSPI tft = TFT_eSPI();

void initUI() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
}

void drawBar(int x, int y, int w, int h, float val, float maxVal, uint16_t color) {
    tft.fillRect(x, y, w, h, TFT_DARKGREY);
    int fill = map(constrain(val, 0, maxVal), 0, maxVal, 0, w);
    tft.fillRect(x, y, fill, h, color);
    tft.drawRect(x, y, w, h, TFT_WHITE);
}

void renderDashboard(SensorData* data) {
    // Header
    tft.fillRect(0, 0, 160, 20, TFT_NAVY);
    tft.setTextColor(TFT_WHITE, TFT_NAVY);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("INDUSTRIAL MONITOR V1.0");

    // === TEMPERATURE ===
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 30);
    tft.print("TEMPERATURE");
    
    // Clear area nilai dulu (biar tidak numpuk)
    tft.fillRect(10, 45, 75, 30, TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(10, 50);
    tft.print(data->temp, 1);
    tft.print(" C");
    
    drawBar(10, 80, 65, 20, data->temp, 100.0, TFT_YELLOW);

    // === PRESSURE ===
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(95, 30);
    tft.print("PRESSURE");
    
    tft.fillRect(95, 45, 70, 30, TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(95, 50);
    tft.print(data->pressure, 1);
    tft.print(" Bar");
    
    drawBar(95, 80, 65, 20, data->pressure, 10.0, TFT_GREEN);

    // === STATUS ===
    tft.fillRect(10, 110, 150, 15, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(10, 110);
    if (data->alarm) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.print("STATUS: CRITICAL     ");
    } else {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.print("STATUS: NORMAL       ");
    }
}

void renderAlarm() {
    bool blink = (millis() / 300) % 2;
    tft.fillScreen(blink ? TFT_RED : TFT_BLACK);
    tft.setTextColor(blink ? TFT_BLACK : TFT_RED, blink ? TFT_RED : TFT_BLACK);
    
    tft.setTextSize(3);
    tft.setCursor(25, 40);
    tft.print("!!!");
    tft.setCursor(15, 70);
    tft.print("ALARM");
    
    tft.setTextSize(2);
    tft.setCursor(20, 105);
    tft.print("CHECK SENSORS");
}