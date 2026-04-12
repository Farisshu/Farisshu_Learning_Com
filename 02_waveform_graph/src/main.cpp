#include <Arduino.h>
#include "display.h"
#include "graph.h"

#define LED_PIN 2

char commandBuffer[32] = "";
int cmdIndex = 0;
unsigned long lastUpdate = 0;

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    
    initDisplay();
    initGraph();
    
    Serial.println("\n✅ Waveform Graph Ready!");
    Serial.println("Type 'help' for commands\n");
}

void loop() {
    // Baca Serial
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (cmdIndex > 0) {
                handleGraphCommand(commandBuffer);
                cmdIndex = 0;
                memset(commandBuffer, 0, 32);
            }
        } else {
            if (cmdIndex < 31) {
                commandBuffer[cmdIndex++] = c;
                commandBuffer[cmdIndex] = '\0';
            }
        }
    }

    // Update data grafik
    updateSignal();

    // Refresh layar (~20 FPS)
    unsigned long now = millis();
    if (now - lastUpdate >= 20) {
        drawGraph(getSignalY);
        lastUpdate = now;
        digitalWrite(LED_PIN, (now / 500) % 2);
    }
}