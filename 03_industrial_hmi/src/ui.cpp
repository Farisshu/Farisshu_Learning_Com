/**
 * @file ui.cpp
 * @brief UI display implementation for Industrial HMI
 * 
 * Implements TFT display rendering functions for dashboard and alarm states.
 */

#include "ui.h"
#include "config.h"

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
static const uint16_t DISPLAY_WIDTH = 160;
static const uint16_t DISPLAY_HEIGHT = 128;
static const uint8_t MARGIN = 8;

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static TFT_eSPI tft = TFT_eSPI();

// ============================================================================
// PRIVATE FUNCTION PROTOTYPES
// ============================================================================
static void drawBar(int x, int y, int w, int h, float val, float maxVal, uint16_t color);

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

/**
 * @brief Initialize the UI display
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initUI(void) {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    
    LOG_INFO("UI", "Display initialized");
    return STATUS_OK;
}

/**
 * @brief Draw a progress bar
 * @param x X position
 * @param y Y position
 * @param w Width
 * @param h Height
 * @param val Current value
 * @param maxVal Maximum value
 * @param color Bar color
 */
static void drawBar(int x, int y, int w, int h, float val, float maxVal, uint16_t color) {
    tft.fillRect(x, y, w, h, TFT_DARKGREY);
    int fill = map(constrain(val, 0, maxVal), 0, maxVal, 0, w);
    tft.fillRect(x, y, fill, h, color);
    tft.drawRect(x, y, w, h, TFT_WHITE);
}

/**
 * @brief Render the main dashboard with sensor data
 * @param data Pointer to SensorData_t structure
 */
void renderDashboard(const SensorData_t* data) {
    if (data == NULL) {
        LOG_ERROR("UI", "NULL data pointer");
        return;
    }

    // Header
    tft.fillRect(0, 0, DISPLAY_WIDTH, 20, TFT_NAVY);
    tft.setTextColor(TFT_WHITE, TFT_NAVY);
    tft.setTextSize(1);
    tft.setCursor(5, 5);
    tft.print("INDUSTRIAL MONITOR V1.0");

    // === TEMPERATURE ===
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(MARGIN, 30);
    tft.print("TEMPERATURE");
    
    // Clear area nilai dulu (biar tidak numpuk)
    tft.fillRect(MARGIN, 45, 75, 30, TFT_BLACK);
    tft.setTextSize(3);
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setCursor(MARGIN, 50);
    tft.print(data->temp, 1);
    tft.print(" C");
    
    drawBar(MARGIN, 80, 65, 20, data->temp, 100.0, TFT_YELLOW);

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
    tft.fillRect(MARGIN, 110, DISPLAY_WIDTH - 2*MARGIN, 15, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(MARGIN, 110);
    if (data->alarm) {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.print("STATUS: CRITICAL     ");
    } else {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.print("STATUS: NORMAL       ");
    }
}

/**
 * @brief Render the alarm screen
 */
void renderAlarm(void) {
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