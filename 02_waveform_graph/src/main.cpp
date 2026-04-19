/**
 * @file main.cpp
 * @brief Waveform Graph - Main Application Entry Point
 * 
 * This application displays real-time waveform graphs on TFT display.
 * Features:
 * - Real-time sine wave generation with noise
 * - Configurable frequency and amplitude
 * - Pause/resume functionality
 * - Serial command interface
 * 
 * FreeRTOS Tasks:
 * - Display Task: Updates TFT display at ~50 FPS
 * - Signal Task: Generates new signal samples
 */

#include <Arduino.h>
#include "display.h"
#include "graph.h"
#include "config.h"

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define LED_PIN                 2
#define DISPLAY_UPDATE_INTERVAL_MS  20  // ~50 FPS
#define SIGNAL_UPDATE_INTERVAL_MS   10
#define COMMAND_BUFFER_SIZE     32
#define DISPLAY_STACK_SIZE      STACK_MEDIUM
#define SIGNAL_STACK_SIZE       STACK_SMALL

// ============================================================================
// PRIVATE TYPES
// ============================================================================
typedef struct {
    char commandBuffer[COMMAND_BUFFER_SIZE];
    int cmdIndex;
} AppState_t;

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static AppState_t s_appState = {0};
static TaskHandle_t xDisplayTaskHandle = NULL;
static TaskHandle_t xSignalTaskHandle = NULL;

// ============================================================================
// PRIVATE FUNCTION PROTOTYPES
// ============================================================================
static void vDisplayTask(void *pvParameters);
static void vSignalTask(void *pvParameters);
static void processSerialCommand(char c);

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

/**
 * @brief Display task - updates graph at fixed frame rate
 */
static void vDisplayTask(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    
    LOG_INFO("MAIN", "Display task started (%d FPS)", 1000/DISPLAY_UPDATE_INTERVAL_MS);
    
    for (;;) {
        drawGraph(getSignalY);
        
        // LED indicator (toggle every 500ms)
        static bool ledState = false;
        ledState = !ledState;
        digitalWrite(LED_PIN, ledState ? HIGH : LOW);
        
        TASK_DELAY_UNTIL(xLastWakeTime, DISPLAY_UPDATE_INTERVAL_MS);
    }
}

/**
 * @brief Signal task - generates new signal samples
 */
static void vSignalTask(void *pvParameters) {
    LOG_INFO("MAIN", "Signal task started");
    
    for (;;) {
        updateSignal();
        TASK_DELAY_MS(SIGNAL_UPDATE_INTERVAL_MS);
    }
}

/**
 * @brief Process serial command character by character
 */
static void processSerialCommand(char c) {
    if (c == '\n' || c == '\r') {
        if (s_appState.cmdIndex > 0) {
            handleGraphCommand(s_appState.commandBuffer);
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
    LOG_INFO("MAIN", "Waveform Graph %s", FW_VERSION_STRING);
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
    
    status = initGraph();
    if (status != STATUS_OK) {
        LOG_ERROR("MAIN", "Graph initialization failed!");
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
        vSignalTask,
        "SignalTask",
        SIGNAL_STACK_SIZE,
        NULL,
        PRIORITY_LOW,
        &xSignalTaskHandle,
        tskNO_AFFINITY
    );
    
    if (xReturn != pdPASS) {
        LOG_ERROR("MAIN", "Failed to create signal task");
    }
    
    Serial.println("\n✅ Waveform Graph Ready!");
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
