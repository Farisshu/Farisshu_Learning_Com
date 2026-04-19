/**
 * @file graph.h
 * @brief Waveform graph generation module
 * 
 * Generates and manages waveform signal data with configurable
 * frequency, amplitude, and noise.
 */

#ifndef GRAPH_H
#define GRAPH_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// PUBLIC CONSTANTS
// ============================================================================
#define GRAPH_WIDTH         160
#define GRAPH_HEIGHT        128
#define GRAPH_CENTER_Y      (GRAPH_HEIGHT / 2)

// ============================================================================
// PUBLIC FUNCTION PROTOTYPES
// ============================================================================

/**
 * @brief Initialize the graph module and clear signal buffer
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initGraph(void);

/**
 * @brief Update signal buffer with new sample (call periodically)
 */
void updateSignal(void);

/**
 * @brief Get Y position of signal at given X coordinate
 * @param x X coordinate (0 to GRAPH_WIDTH-1)
 * @return uint16_t Y position on screen
 */
uint16_t getSignalY(uint8_t x);

/**
 * @brief Set waveform frequency
 * @param hz Frequency in Hz (0.1 to 10.0)
 */
void setFrequency(float hz);

/**
 * @brief Set waveform amplitude
 * @param amp Amplitude value (5 to 55)
 */
void setAmplitude(float amp);

/**
 * @brief Toggle pause state of signal generation
 * @return bool New pause state
 */
bool togglePause(void);

/**
 * @brief Get current pause state
 * @return bool True if paused
 */
bool isPaused(void);

/**
 * @brief Handle serial command for graph control
 * @param cmd Command string from serial input
 */
void handleGraphCommand(const char* cmd);

#ifdef __cplusplus
}
#endif

#endif /* GRAPH_H */