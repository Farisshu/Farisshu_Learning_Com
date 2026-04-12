#include "display.h"

TFT_eSPI tft = TFT_eSPI();

void initDisplay() {
    tft.init();
    tft.setRotation(1); // Landscape 160x128
    tft.fillScreen(TFT_BLACK);
    
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(30, 50);
    tft.print("WAVEFORM");
    tft.setCursor(45, 75);
    tft.print("GRAPH");
    delay(1000);
}

void drawGrid() {
    // Garis tengah
    tft.drawFastHLine(0, 64, 160, tft.color565(30, 30, 50));
    // Marker vertikal
    for (int x = 0; x < 160; x += 40) {
        tft.drawFastVLine(x, 0, 128, tft.color565(30, 30, 50));
    }
}

void drawGraph(uint16_t (*getY)(uint8_t)) {
    tft.fillScreen(TFT_BLACK);
    drawGrid();

    // Gambar grafik (optimasi SPI)
    tft.startWrite();
    for (uint8_t x = 0; x < 159; x++) {
        uint16_t y1 = getY(x);
        uint16_t y2 = getY(x + 1);
        tft.drawLine(x, y1, x + 1, y2, TFT_CYAN);
    }
    tft.endWrite();

    // Info overlay
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("CH1: SIMULATED_SENSOR");
}