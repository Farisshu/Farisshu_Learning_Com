/**
 * @file simulator.cpp
 * @brief Motor simulation module implementation
 * 
 * Simulates motor RPM behavior with smooth transitions and noise.
 */

#include "simulator.h"
#include <Arduino.h>
#include <string.h>

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define RPM_MIN             0.0f
#define RPM_MAX             3000.0f
#define RPM_STEP            5.0f
#define RPM_NOISE           20.0f
#define AUTO_INTERVAL_MIN   5000
#define AUTO_INTERVAL_MAX   10000
#define AUTO_RPM_MIN        500
#define AUTO_RPM_MAX        2500

// ============================================================================
// PRIVATE TYPES
// ============================================================================
typedef struct {
    float currentRpm;
    float targetRpm;
    bool autoMode;
    unsigned long lastAutoChange;
    unsigned long autoInterval;
} SimulatorState_t;

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static SimulatorState_t s_simState = {0};

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

/**
 * @brief Generate random RPM value within auto mode range
 * @return float Random RPM value
 */
static float generateRandomRPM(void) {
    return (float)random(AUTO_RPM_MIN, AUTO_RPM_MAX);
}

/**
 * @brief Generate random interval for auto mode
 * @return unsigned long Random interval in milliseconds
 */
static unsigned long generateRandomInterval(void) {
    return random(AUTO_INTERVAL_MIN, AUTO_INTERVAL_MAX);
}

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

SystemStatus_t initSimulator(void) {
    LOG_INFO("SIMULATOR", "Initializing simulator...");
    
    s_simState.currentRpm = 0.0f;
    s_simState.targetRpm = 0.0f;
    s_simState.autoMode = false;
    s_simState.lastAutoChange = millis();
    s_simState.autoInterval = AUTO_INTERVAL_MIN;
    
    LOG_INFO("SIMULATOR", "Simulator initialized successfully");
    return STATUS_OK;
}

void updateSimulator(void) {
    // Auto-simulation mode
    if (s_simState.autoMode) {
        unsigned long now = millis();
        
        if (now - s_simState.lastAutoChange >= s_simState.autoInterval) {
            s_simState.targetRpm = generateRandomRPM();
            s_simState.lastAutoChange = now;
            s_simState.autoInterval = generateRandomInterval();
            
            LOG_DEBUG("SIMULATOR", "Auto mode: New target RPM = %.1f", s_simState.targetRpm);
        }
    }
    
    // Smooth transition (±RPM_STEP per update)
    float diff = s_simState.currentRpm - s_simState.targetRpm;
    
    if (abs(diff) > RPM_STEP) {
        if (s_simState.currentRpm < s_simState.targetRpm) {
            s_simState.currentRpm += RPM_STEP;
        } else {
            s_simState.currentRpm -= RPM_STEP;
        }
    }
}

float getCurrentRPM(void) {
    // Add small noise (±RPM_NOISE) for realism
    float noise = (float)random(-(int)RPM_NOISE, (int)RPM_NOISE);
    return s_simState.currentRpm + noise;
}

void setTargetRPM(float rpm) {
    s_simState.targetRpm = constrain(rpm, RPM_MIN, RPM_MAX);
    s_simState.autoMode = false;  // Disable auto mode
    
    LOG_INFO("SIMULATOR", "Target RPM set to: %.1f (auto mode OFF)", s_simState.targetRpm);
}

bool toggleAutoMode(void) {
    s_simState.autoMode = !s_simState.autoMode;
    
    if (s_simState.autoMode) {
        s_simState.lastAutoChange = millis();
        s_simState.autoInterval = generateRandomInterval();
        LOG_INFO("SIMULATOR", "Auto mode ON");
    } else {
        LOG_INFO("SIMULATOR", "Auto mode OFF");
    }
    
    return s_simState.autoMode;
}

bool isAutoMode(void) {
    return s_simState.autoMode;
}

void handleCommand(const char* cmd) {
    if (cmd == NULL) {
        LOG_ERROR("SIMULATOR", "NULL command received");
        return;
    }
    
    char buffer[32];
    strncpy(buffer, cmd, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';
    
    // Parse command: "rpm <value>"
    if (strncmp(buffer, "rpm ", 4) == 0) {
        float value = atof(buffer + 4);
        setTargetRPM(value);
        Serial.print("✓ Target RPM: ");
        Serial.println(value);
    }
    else if (strcmp(buffer, "auto") == 0) {
        bool mode = toggleAutoMode();
        Serial.print("✓ Auto simulation: ");
        Serial.println(mode ? "ON" : "OFF");
    }
    else if (strcmp(buffer, "help") == 0) {
        Serial.println("\n=== COMMANDS ===");
        Serial.println("rpm <value>  - Set RPM (0-3000)");
        Serial.println("auto         - Toggle auto mode");
        Serial.println("help         - Show help");
        Serial.println("status       - Show status");
        Serial.println("==============\n");
    }
    else if (strcmp(buffer, "status") == 0) {
        Serial.print("Current: ");
        Serial.print(getCurrentRPM(), 1);
        Serial.print(" | Target: ");
        Serial.print(s_simState.targetRpm, 1);
        Serial.print(" | Auto: ");
        Serial.println(s_simState.autoMode ? "ON" : "OFF");
    }
    else {
        Serial.print("✗ Unknown: ");
        Serial.println(buffer);
        Serial.println("Type 'help' for commands");
    }
}