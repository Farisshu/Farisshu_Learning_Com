/**
 * @file graph.cpp
 * @brief Waveform graph generation module implementation
 * 
 * Generates and manages waveform signal data with configurable
 * frequency, amplitude, and noise.
 */

#include "graph.h"
#include <Arduino.h>
#include <math.h>
#include <string.h>

// ============================================================================
// PRIVATE CONSTANTS
// ============================================================================
#define SIGNAL_UPDATE_INTERVAL_MS   20
#define NOISE_LEVEL                 0.2f
#define MIN_FREQUENCY               0.1f
#define MAX_FREQUENCY               10.0f
#define MIN_AMPLITUDE               5.0f
#define MAX_AMPLITUDE               55.0f

// ============================================================================
// PRIVATE TYPES
// ============================================================================
typedef struct {
    float signalBuffer[GRAPH_WIDTH];
    float phase;
    float frequency;
    float amplitude;
    bool paused;
    unsigned long lastUpdate;
} GraphState_t;

// ============================================================================
// PRIVATE VARIABLES
// ============================================================================
static GraphState_t s_graphState = {0};

// ============================================================================
// PRIVATE FUNCTIONS
// ============================================================================

/**
 * @brief Generate new signal sample with sine wave and noise
 * @return float New Y value for signal buffer
 */
static float generateSignalSample(void) {
    // Calculate sine value
    float sineVal = sin(s_graphState.phase) * s_graphState.amplitude;
    
    // Add noise (±NOISE_LEVEL * amplitude)
    float noiseVal = ((float)random(-100, 100) / 100.0f) * (s_graphState.amplitude * NOISE_LEVEL);
    
    // Center at GRAPH_CENTER_Y
    float newVal = (float)GRAPH_CENTER_Y + sineVal + noiseVal;
    
    // Clamp to screen bounds (with margin)
    return constrain(newVal, 5.0f, (float)(GRAPH_HEIGHT - 5));
}

// ============================================================================
// PUBLIC FUNCTIONS
// ============================================================================

SystemStatus_t initGraph(void) {
    LOG_INFO("GRAPH", "Initializing graph module...");
    
    // Clear signal buffer to center
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        s_graphState.signalBuffer[i] = (float)GRAPH_CENTER_Y;
    }
    
    s_graphState.phase = 0.0f;
    s_graphState.frequency = 2.0f;
    s_graphState.amplitude = 40.0f;
    s_graphState.paused = false;
    s_graphState.lastUpdate = 0;
    
    LOG_INFO("GRAPH", "Graph initialized: Freq=%.1f Hz, Amp=%.1f", 
             s_graphState.frequency, s_graphState.amplitude);
    return STATUS_OK;
}

void updateSignal(void) {
    if (s_graphState.paused) {
        return;
    }
    
    unsigned long now = millis();
    if (now - s_graphState.lastUpdate < SIGNAL_UPDATE_INTERVAL_MS) {
        return;
    }
    s_graphState.lastUpdate = now;

    // Shift buffer left (scrolling effect)
    for (int i = 0; i < (GRAPH_WIDTH - 1); i++) {
        s_graphState.signalBuffer[i] = s_graphState.signalBuffer[i + 1];
    }

    // Update phase for sine wave
    s_graphState.phase += (2.0f * PI * s_graphState.frequency * (float)SIGNAL_UPDATE_INTERVAL_MS) / 1000.0f;
    if (s_graphState.phase > (2.0f * PI)) {
        s_graphState.phase -= (2.0f * PI);
    }

    // Generate and store new sample
    s_graphState.signalBuffer[GRAPH_WIDTH - 1] = generateSignalSample();
}

uint16_t getSignalY(uint8_t x) {
    if (x >= GRAPH_WIDTH) {
        return GRAPH_CENTER_Y;
    }
    return (uint16_t)s_graphState.signalBuffer[x];
}

void setFrequency(float hz) {
    s_graphState.frequency = constrain(hz, MIN_FREQUENCY, MAX_FREQUENCY);
    LOG_INFO("GRAPH", "Frequency set to: %.1f Hz", s_graphState.frequency);
}

void setAmplitude(float amp) {
    s_graphState.amplitude = constrain(amp, MIN_AMPLITUDE, MAX_AMPLITUDE);
    LOG_INFO("GRAPH", "Amplitude set to: %.1f", s_graphState.amplitude);
}

bool togglePause(void) {
    s_graphState.paused = !s_graphState.paused;
    LOG_INFO("GRAPH", "Pause mode: %s", s_graphState.paused ? "ON" : "OFF");
    return s_graphState.paused;
}

bool isPaused(void) {
    return s_graphState.paused;
}

void handleGraphCommand(const char* cmd) {
    if (cmd == NULL) {
        LOG_ERROR("GRAPH", "NULL command received");
        return;
    }
    
    char buffer[32];
    strncpy(buffer, cmd, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    if (strncmp(buffer, "freq ", 5) == 0) {
        setFrequency(atof(buffer + 5));
        Serial.print("✓ Freq: ");
        Serial.println(s_graphState.frequency, 1);
    }
    else if (strncmp(buffer, "amp ", 4) == 0) {
        setAmplitude(atof(buffer + 4));
        Serial.print("✓ Amp: ");
        Serial.println(s_graphState.amplitude, 1);
    }
    else if (strcmp(buffer, "pause") == 0) {
        Serial.print("✓ Pause: ");
        Serial.println(togglePause() ? "ON" : "OFF");
    }
    else if (strcmp(buffer, "help") == 0) {
        Serial.println("\n=== GRAPH COMMANDS ===");
        Serial.println("freq <0.1-10>  - Set waveform frequency");
        Serial.println("amp <5-55>     - Set amplitude");
        Serial.println("pause          - Freeze/unfreeze graph");
        Serial.println("help           - Show this help");
        Serial.println("======================\n");
    }
    else {
        Serial.print("✗ Unknown: ");
        Serial.println(buffer);
        Serial.println("Type 'help' for commands");
    }
}
