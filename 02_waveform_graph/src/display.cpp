/**
 * @file display.cpp
 * @brief Display module implementation for Waveform Graph
 * 
 * Handles TFT display initialization and waveform graph rendering.
 */

#include "display.h"
#include <Arduino.h>

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static TFT_eSPI tft;  // Display instance

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

SystemStatus_t initDisplay(void) {
    LOG_INFO("DISPLAY", "Initializing display...");
    
    tft.init();
    tft.setRotation(1);  // Landscape 160x128
    tft.fillScreen(TFT_BLACK);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(30, 50);
    tft.print("WAVEFORM");
    tft.setCursor(45, 75);
    tft.print("GRAPH");
    
    TASK_DELAY_MS(1000);
    
    LOG_INFO("DISPLAY", "Display initialized successfully");
    return STATUS_OK;
}

void drawGrid(void) {
    // Center horizontal line
    tft.drawFastHLine(0, GRAPH_CENTER_Y, GRAPH_WIDTH, tft.color565(30, 30, 50));
    
    // Vertical markers
    for (int x = 0; x < GRAPH_WIDTH; x += 40) {
        tft.drawFastVLine(x, 0, GRAPH_HEIGHT, tft.color565(30, 30, 50));
    }
}

void drawGraph(uint16_t (*getY)(uint8_t)) {
    tft.fillScreen(TFT_BLACK);
    drawGrid();

    // Draw waveform (optimized SPI)
    tft.startWrite();
    for (uint8_t x = 0; x < (GRAPH_WIDTH - 1); x++) {
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
