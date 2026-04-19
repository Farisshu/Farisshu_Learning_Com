/**
 * @file main.cpp
 * @brief Main application for Industrial HMI
 * 
 * Industrial monitoring system with TFT display, sensor simulation,
 * and alarm management. Supports serial commands for testing.
 */

#include <Arduino.h>
#include "../../include/config.h"
#include "ui.h"
#include "logic.h"

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define MAX_TEMP            60.0f
#define MAX_PRESS           5.0f
#define UI_UPDATE_INTERVAL  100
#define SERIAL_BUFFER_SIZE  32

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static char serialBuffer[SERIAL_BUFFER_SIZE] = "";
static int serialIdx = 0;

// ============================================================================
// PRIVATE FUNCTION PROTOTYPES
// ============================================================================
static void processSerialCommand(void);

// ============================================================================
// ARDUINO SETUP
// ============================================================================

/**
 * @brief Arduino setup function - runs once at startup
 */
void setup(void) {
    Serial.begin(115200);
    
    LOG_INFO("MAIN", "Industrial HMI starting...");
    
    // Initialize modules
    if (initUI() != STATUS_OK) {
        LOG_ERROR("MAIN", "UI initialization failed!");
    }
    
    if (initLogic() != STATUS_OK) {
        LOG_ERROR("MAIN", "Logic initialization failed!");
    }
    
    LOG_INFO("MAIN", "✅ Industrial HMI Ready. Ketik 'temp 70' untuk tes alarm.");
}

// ============================================================================
// ARDUINO LOOP
// ============================================================================

/**
 * @brief Arduino loop function - runs continuously
 */
void loop(void) {
    // 1. Update Data Sensor
    updateSensorData();
    SensorData_t* data = getData();
    bool isAlarm = checkAlarmStatus(MAX_TEMP, MAX_PRESS);

    // 2. Baca Serial (Anti-Spam)
    processSerialCommand();

    // 3. Render UI (Non-Blocking Timer)
    static unsigned long lastDraw = 0;
    if (millis() - lastDraw >= UI_UPDATE_INTERVAL) {
        if (isAlarm) {
            renderAlarm();
        } else {
            renderDashboard(data);
        }
        lastDraw = millis();
    }
    
    // Small delay to prevent watchdog trigger
    delay(1);
}

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

/**
 * @brief Process serial commands
 */
static void processSerialCommand(void) {
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (serialIdx > 0) {
                serialBuffer[serialIdx] = '\0';
                handleLogicCommand(serialBuffer, getData());
            }
            serialIdx = 0;
        } else {
            if (serialIdx < (SERIAL_BUFFER_SIZE - 1)) {
                serialBuffer[serialIdx++] = c;
            }
        }
    }
}