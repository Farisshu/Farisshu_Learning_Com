/**
 * @file main.cpp
 * @brief Motor Monitor - Main Application Entry Point
 * 
 * This application monitors and simulates motor RPM with TFT display.
 * Features:
 * - Real-time RPM simulation with smooth transitions
 * - TFT display visualization with gradient background
 * - Serial command interface for control
 * - Auto-mode for automatic RPM changes
 * 
 * FreeRTOS Tasks:
 * - Display Task: Updates TFT display periodically
 * - Simulator Task: Updates motor simulation state
 * - Command Task: Handles serial input commands
 */

#include <Arduino.h>
#include "display.h"
#include "simulator.h"
#include "config.h"

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define LED_PIN             2
#define UPDATE_INTERVAL_MS  100
#define COMMAND_BUFFER_SIZE 32
#define DISPLAY_STACK_SIZE  STACK_MEDIUM
#define SIMULATOR_STACK_SIZE STACK_SMALL

// ============================================================================
// PRIVATE TYPES
// ============================================================================
typedef struct {
    char commandBuffer[COMMAND_BUFFER_SIZE];
    int cmdIndex;
    unsigned long lastUpdate;
} AppState_t;

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static AppState_t s_appState = {0};
static TaskHandle_t xDisplayTaskHandle = NULL;
static TaskHandle_t xSimulatorTaskHandle = NULL;

// ============================================================================
// PRIVATE FUNCTION PROTOTYPES
// ============================================================================
static void vDisplayTask(void *pvParameters);
static void vSimulatorTask(void *pvParameters);
static void processSerialCommand(char c);

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

/**
 * @brief Display task - runs at fixed interval to update screen
 */
static void vDisplayTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    LOG_INFO("MAIN", "Display task started");
    
    for (;;) {
        float displayRPM = getCurrentRPM();
        drawMotorDisplay(displayRPM);
        
        // LED indicator (toggle every 500ms)
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        
        TASK_DELAY_UNTIL(xLastWakeTime);
    }
}

/**
 * @brief Simulator task - updates motor simulation state
 */
static void vSimulatorTask(void *pvParameters) {
    LOG_INFO("MAIN", "Simulator task started");
    
    for (;;) {
        updateSimulator();
        TASK_DELAY_MS(UPDATE_INTERVAL_MS / 2);
    }
}

/**
 * @brief Process serial command character by character
 */
static void processSerialCommand(char c) {
    if (c == '\n' || c == '\r') {
        if (s_appState.cmdIndex > 0) {
            handleCommand(s_appState.commandBuffer);
            s_appState.cmdIndex = 0;
            memset(s_appState.commandBuffer, 0, COMMAND_BUFFER_SIZE);
        }
    } else {
        if (s_appState.cmdIndex < (COMMAND_BUFFER_SIZE - 1)) {
            s_appState.commandBuffer[s_appState.cmdIndex++] = c;
            s_appState.commandBuffer[s_appState.cmdIndex] = '\0';
        }
    }
}

// ============================================================================
// ARDUINO SETUP & LOOP
// ============================================================================

void setup() {
    Serial.begin(115200);
    
    // Wait for serial connection
    delay(500);
    
    LOG_INFO("MAIN", "=================================");
    LOG_INFO("MAIN", "Motor Monitor %s", FW_VERSION_STRING);
    LOG_INFO("MAIN", "=================================");
    
    // Init hardware
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    
    // Init modules
    SystemStatus_t status;
    
    status = initDisplay();
    if (status != STATUS_OK) {
        LOG_ERROR("MAIN", "Display initialization failed!");
    }
    
    status = initSimulator();
    if (status != STATUS_OK) {
        LOG_ERROR("MAIN", "Simulator initialization failed!");
    }
    
    // Create FreeRTOS tasks
    BaseType_t xReturn;
    
    xReturn = xTaskCreatePinnedToCore(
        vDisplayTask,
        "DisplayTask",
        DISPLAY_STACK_SIZE,
        NULL,
        PRIORITY_NORMAL,
        &xDisplayTaskHandle,
        tskNO_AFFINITY
    );
    
    if (xReturn != pdPASS) {
        LOG_ERROR("MAIN", "Failed to create display task");
    }
    
    xReturn = xTaskCreatePinnedToCore(
        vSimulatorTask,
        "SimulatorTask",
        SIMULATOR_STACK_SIZE,
        NULL,
        PRIORITY_LOW,
        &xSimulatorTaskHandle,
        tskNO_AFFINITY
    );
    
    if (xReturn != pdPASS) {
        LOG_ERROR("MAIN", "Failed to create simulator task");
    }
    
    Serial.println("\n✅ Motor Monitor Ready!");
    Serial.println("Type 'help' for commands\n");
    
    LOG_INFO("MAIN", "System initialized successfully");
}

void loop() {
    // Read and process serial input (runs on core 0)
    while (Serial.available() > 0) {
        char c = Serial.read();
        processSerialCommand(c);
    }
    
    // Small delay to prevent busy-waiting
    TASK_DELAY_MS(10);
}