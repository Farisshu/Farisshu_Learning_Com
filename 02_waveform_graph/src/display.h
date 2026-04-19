/**
 * @file display.h
 * @brief Display module for Waveform Graph
 * 
 * Handles TFT display initialization and waveform graph rendering.
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include "config.h"
#include "graph.h"

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
 * @brief Draw waveform graph using callback function for Y values
 * @param getY Function pointer to get Y value at given X position
 */
void drawGraph(uint16_t (*getY)(uint8_t));

/**
 * @brief Draw grid lines on display
 */
void drawGrid(void);

#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H */