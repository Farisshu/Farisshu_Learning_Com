/**
 * @file ui.h
 * @brief UI display module for Industrial HMI
 * 
 * Handles TFT display rendering for dashboard and alarm states.
 */

#ifndef UI_H
#define UI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <TFT_eSPI.h>
#include "logic.h"

/**
 * @brief Initialize the UI display
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initUI(void);

/**
 * @brief Render the main dashboard with sensor data
 * @param data Pointer to SensorData structure
 */
void renderDashboard(const SensorData_t* data);

/**
 * @brief Render the alarm screen
 */
void renderAlarm(void);

#ifdef __cplusplus
}
#endif

#endif /* UI_H */