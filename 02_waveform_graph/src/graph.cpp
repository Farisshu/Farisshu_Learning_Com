#include "graph.h"
#include <math.h>

static float signalBuffer[GRAPH_WIDTH];
static float phase = 0;
static float frequency = 2.0;
static float amplitude = 40.0;
static bool paused = false;
static unsigned long lastUpdate = 0;
static const unsigned long updateInterval = 20; // ~50 FPS

void initGraph() {
    for (int i = 0; i < GRAPH_WIDTH; i++) {
        signalBuffer[i] = 64; // Tengah layar (128/2)
    }
}

void updateSignal() {
    if (paused) return;
    unsigned long now = millis();
    if (now - lastUpdate < updateInterval) return;
    lastUpdate = now;

    // Geser buffer ke kiri (scrolling)
    for (int i = 0; i < GRAPH_WIDTH - 1; i++) {
        signalBuffer[i] = signalBuffer[i + 1];
    }

    // Hitung nilai baru (Sine + Noise)
    phase += (2 * PI * frequency * updateInterval) / 1000.0;
    if (phase > 2 * PI) phase -= 2 * PI;

    float sineVal = sin(phase) * amplitude;
    float noiseVal = (random(-100, 100) / 100.0) * (amplitude * 0.2);
    float newVal = 64.0 + sineVal + noiseVal;

    // Clamp agar tidak keluar layar
    signalBuffer[GRAPH_WIDTH - 1] = constrain(newVal, 5, 123);
}

uint16_t getSignalY(uint8_t x) {
    return (uint16_t)signalBuffer[x];
}

void setFrequency(float hz) {
    frequency = constrain(hz, 0.1, 10.0);
}

void setAmplitude(float amp) {
    amplitude = constrain(amp, 5, 55);
}

bool togglePause() {
    paused = !paused;
    return paused;
}

bool isPaused() { return paused; }

void handleGraphCommand(const char* cmd) {
    char buffer[32];
    strncpy(buffer, cmd, 31);
    buffer[31] = '\0';

    if (strncmp(buffer, "freq ", 5) == 0) {
        setFrequency(atof(buffer + 5));
        Serial.print("✓ Freq: "); Serial.println(frequency);
    } 
    else if (strncmp(buffer, "amp ", 4) == 0) {
        setAmplitude(atof(buffer + 4));
        Serial.print("✓ Amp: "); Serial.println(amplitude);
    } 
    else if (strcmp(buffer, "pause") == 0) {
        Serial.print("✓ Pause: "); Serial.println(togglePause() ? "ON" : "OFF");
    } 
    else if (strcmp(buffer, "help") == 0) {
        Serial.println("\n=== GRAPH COMMANDS ===");
        Serial.println("freq <0.1-10>  - Atur frekuensi gelombang");
        Serial.println("amp <5-55>     - Atur amplitudo");
        Serial.println("pause          - Freeze / lanjutkan grafik");
        Serial.println("help           - Bantuan");
        Serial.println("========================\n");
    } 
    else {
        Serial.print("✗ Unknown: "); Serial.println(buffer);
    }
}