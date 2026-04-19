/**
 * @file ui_display.h
 * @brief UI Display Module for RS485 Slave Node
 * 
 * Handles TFT display rendering for sensor data visualization
 * with static layout and dynamic value updates.
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

#include <TFT_eSPI.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Initialize the display module
 */
void ui_init(void);

/**
 * @brief Update display with new sensor values
 * @param temp Temperature value in Celsius
 * @param press Pressure value in Bar
 * @param linkActive Link status flag (true = active)
 */
void ui_update(float temp, float press, bool linkActive);

/**
 * @brief Draw static layout elements (headers, labels)
 */
void ui_drawStaticLayout(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_DISPLAY_H */