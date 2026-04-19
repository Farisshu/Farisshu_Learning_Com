/**
 * @file logic.cpp
 * @brief Sensor data logic implementation for Industrial HMI
 * 
 * Implements sensor data simulation, alarm checking, and command parsing.
 */

#include "logic.h"
#include <Arduino.h>
#include "config.h"

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define DEFAULT_TEMP        25.0f
#define DEFAULT_PRESSURE    1.0f
#define MIN_TEMP            20.0f
#define MIN_PRESSURE        0.5f
#define UPDATE_INTERVAL_MS  100

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static SensorData_t currentData = {DEFAULT_TEMP, DEFAULT_PRESSURE, false};
static unsigned long lastUpdate = 0;

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

/**
 * @brief Initialize the logic module
 * @return SystemStatus_t STATUS_OK on success
 */
SystemStatus_t initLogic(void) {
    currentData.temp = DEFAULT_TEMP;
    currentData.pressure = DEFAULT_PRESSURE;
    currentData.alarm = false;
    
    LOG_INFO("LOGIC", "Logic module initialized (T=%.1f, P=%.1f)", 
             (double)currentData.temp, (double)currentData.pressure);
    return STATUS_OK;
}

/**
 * @brief Update sensor data (simulation)
 */
void updateSensorData(void) {
    // Update tiap 100ms
    if (millis() - lastUpdate < UPDATE_INTERVAL_MS) {
        return;
    }
    lastUpdate = millis();

    // Simulasi perubahan acak (Random Walk)
    currentData.temp += random(-30, 30) / 10.0f;
    currentData.pressure += random(-5, 5) / 10.0f;

    // Batas minimum agar tidak minus (fisik)
    if (currentData.temp < MIN_TEMP) {
        currentData.temp = MIN_TEMP;
    }
    if (currentData.pressure < MIN_PRESSURE) {
        currentData.pressure = MIN_PRESSURE;
    }
}

/**
 * @brief Check if alarm conditions are met
 * @param limitTemp Temperature limit threshold
 * @param limitPressure Pressure limit threshold
 * @return true if alarm condition is active, false otherwise
 */
bool checkAlarmStatus(float limitTemp, float limitPressure) {
    if (currentData.temp > limitTemp || currentData.pressure > limitPressure) {
        currentData.alarm = true;
        return true;
    }
    currentData.alarm = false;
    return false;
}

/**
 * @brief Get pointer to current sensor data
 * @return Pointer to SensorData_t structure
 */
SensorData_t* getData(void) {
    return &currentData;
}

/**
 * @brief Handle logic commands from serial input
 * @param cmd Command string (e.g., "temp 70", "press 5")
 * @param data Pointer to SensorData_t structure to update
 */
void handleLogicCommand(const char* cmd, SensorData_t* data) {
    if (cmd == NULL || data == NULL) {
        LOG_ERROR("LOGIC", "NULL parameter in handleLogicCommand");
        return;
    }

    char buffer[32];
    strncpy(buffer, cmd, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (strncmp(buffer, "temp ", 5) == 0) {
        data->temp = atof(buffer + 5);
        LOG_INFO("LOGIC", "Simulasi Suhu diset ke: %.1f", (double)data->temp);
    } else if (strncmp(buffer, "press ", 6) == 0) {
        data->pressure = atof(buffer + 6);
        LOG_INFO("LOGIC", "Simulasi Tekanan diset ke: %.1f", (double)data->pressure);
    } else {
        LOG_WARN("LOGIC", "Cmd Logic: temp <val>, press <val>");
    }
}