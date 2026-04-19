/**
 * @file ui_display.cpp
 * @brief UI Display Implementation for RS485 Slave Node
 * 
 * Implements efficient TFT display rendering with static layout
 * and dynamic value updates for sensor monitoring.
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#include "ui_display.h"

/* Include global configuration */
#include "config.h"

/**
 * @brief Private constants for display layout
 */
#define DISPLAY_WIDTH       160
#define DISPLAY_HEIGHT      128
#define DISPLAY_MARGIN      8
#define TITLE_Y_POS         12
#define TEMP_Y_POS          42
#define PRESS_Y_POS         82
#define STATUS_Y_POS        114

/**
 * @brief Display instance
 */
static TFT_eSPI tft;

/**
 * @brief Initialize the display module
 */
void ui_init(void) {
    tft.init();
    tft.setRotation(1); // Landscape orientation
    tft.fillScreen(TFT_BLACK);
    ui_drawStaticLayout();
}

/**
 * @brief Draw static layout elements (headers, labels)
 */
void ui_drawStaticLayout(void) {
    /* Draw title */
    tft.setTextColor(TFT_CYAN, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(DISPLAY_MARGIN, TITLE_Y_POS);
    tft.print("RS485 MONITORING NODE");

    /* Draw temperature label */
    tft.setTextColor(TFT_WHITE, TFT_BLACK);
    tft.setTextSize(1);
    tft.setCursor(DISPLAY_MARGIN, TEMP_Y_POS - 14);
    tft.print("TEMPERATURE");

    /* Draw pressure label */
    tft.setCursor(DISPLAY_MARGIN, PRESS_Y_POS - 14);
    tft.print("PRESSURE");
}

/**
 * @brief Update display with new sensor values
 */
void ui_update(float temp, float press, bool linkActive) {
    /* Clear value areas only (efficient update) */
    tft.fillRect(DISPLAY_MARGIN, TEMP_Y_POS, 100, 24, TFT_BLACK);
    tft.fillRect(DISPLAY_MARGIN, PRESS_Y_POS, 100, 24, TFT_BLACK);
    tft.fillRect(DISPLAY_WIDTH - 60, STATUS_Y_POS, 55, 14, TFT_BLACK);

    /* Draw temperature value */
    tft.setTextColor(TFT_YELLOW, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(DISPLAY_MARGIN, TEMP_Y_POS);
    tft.print(temp, 1);
    tft.setTextSize(1);
    tft.print(" C");

    /* Draw pressure value */
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setTextSize(2);
    tft.setCursor(DISPLAY_MARGIN, PRESS_Y_POS);
    tft.print(press, 1);
    tft.setTextSize(1);
    tft.print(" Bar");

    /* Draw link status */
    tft.setTextSize(1);
    if (linkActive) {
        tft.setTextColor(TFT_GREEN, TFT_BLACK);
        tft.setCursor(DISPLAY_WIDTH - 55, STATUS_Y_POS);
        tft.print("LINK OK");
    } else {
        tft.setTextColor(TFT_RED, TFT_BLACK);
        tft.setCursor(DISPLAY_WIDTH - 60, STATUS_Y_POS);
        tft.print("OFFLINE");
    }
}