/**
 * @file logic.h
 * @brief Sensor data logic and alarm management for Industrial HMI
 * 
 * Handles sensor data simulation, alarm checking, and command parsing.
 */

#ifndef LOGIC_H
#define LOGIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief Sensor data structure
 */
typedef struct {
    float temp;       /**< Temperature value in Celsius */
    float pressure;   /**< Pressure value in Bar */
    bool alarm;       /**< Alarm status flag */
} SensorData_t;

/**
 * @brief Initialize the logic module
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initLogic(void);

/**
 * @brief Update sensor data (simulation)
 */
void updateSensorData(void);

/**
 * @brief Check if alarm conditions are met
 * @param limitTemp Temperature limit threshold
 * @param limitPressure Pressure limit threshold
 * @return true if alarm condition is active, false otherwise
 */
bool checkAlarmStatus(float limitTemp, float limitPressure);

/**
 * @brief Get pointer to current sensor data
 * @return Pointer to SensorData_t structure
 */
SensorData_t* getData(void);

/**
 * @brief Handle logic commands from serial input
 * @param cmd Command string (e.g., "temp 70", "press 5")
 * @param data Pointer to SensorData structure to update
 */
void handleLogicCommand(const char* cmd, SensorData_t* data);

#ifdef __cplusplus
}
#endif

#endif /* LOGIC_H */