/**
 * @file display.h
 * @brief Display module for Motor Monitor
 * 
 * Handles TFT display initialization and motor RPM visualization.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// PUBLIC FUNCTION PROTOTYPES
// ============================================================================

/**
 * @brief Initialize the TFT display
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initDisplay(void);

/**
 * @brief Draw motor RPM on display with gradient background
 * @param rpm Current motor RPM value
 */
void drawMotorDisplay(float rpm);

/**
 * @brief Clear screen with gradient background
 */
void clearScreenWithGradient(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H */