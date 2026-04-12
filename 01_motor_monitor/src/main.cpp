#include <Arduino.h>
#include "display.h"
#include "simulator.h"

#define LED_PIN 2
#define UPDATE_INTERVAL 100  // Update tiap 100ms

char commandBuffer[32] = "";
int cmdIndex = 0;
unsigned long lastUpdate = 0;

void setup() {
  Serial.begin(115200);
  
  // Init hardware
  pinMode(LED_PIN, OUTPUT);
  
  // Init modules
  initDisplay();
  initSimulator();
  
  Serial.println("\n✅ Motor Monitor Ready!");
  Serial.println("Type 'help' for commands\n");
}

void loop() {
  // Read serial input
  while (Serial.available() > 0) {
    char c = Serial.read();
    
    if (c == '\n' || c == '\r') {
      if (cmdIndex > 0) {
        handleCommand(commandBuffer);
        cmdIndex = 0;
        memset(commandBuffer, 0, 32);
      }
    }
    else {
      if (cmdIndex < 31) {
        commandBuffer[cmdIndex++] = c;
        commandBuffer[cmdIndex] = '\0';
      }
    }
  }
  
  // Update simulator
  updateSimulator();
  
  // Update display
  unsigned long now = millis();
  if (now - lastUpdate >= UPDATE_INTERVAL) {
    float displayRPM = getCurrentRPM();
    drawMotorDisplay(displayRPM);
    lastUpdate = now;
    
    // LED indicator
    digitalWrite(LED_PIN, (now / 500) % 2);
  }
}