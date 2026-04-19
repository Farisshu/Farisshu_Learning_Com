/**
 * @file simulator.h
 * @brief Motor simulation module
 * 
 * Simulates motor RPM behavior with smooth transitions and noise.
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "config.h"

#ifdef __cplusplus
extern "C" {
#endif

// ============================================================================
// PUBLIC FUNCTION PROTOTYPES
// ============================================================================

/**
 * @brief Initialize the motor simulator
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initSimulator(void);

/**
 * @brief Update simulator state (call periodically)
 */
void updateSimulator(void);

/**
 * @brief Get current simulated RPM with noise
 * @return float Current RPM value
 */
float getCurrentRPM(void);

/**
 * @brief Set target RPM for simulation
 * @param rpm Target RPM value (0-3000)
 */
void setTargetRPM(float rpm);

/**
 * @brief Toggle auto simulation mode
 * @return bool New auto mode status
 */
bool toggleAutoMode(void);

/**
 * @brief Get current auto mode status
 * @return bool True if auto mode is enabled
 */
bool isAutoMode(void);

/**
 * @brief Handle serial command input
 * @param cmd Command string from serial
 */
void handleCommand(const char* cmd);

#ifdef __cplusplus
}
#endif

#endif /* SIMULATOR_H */