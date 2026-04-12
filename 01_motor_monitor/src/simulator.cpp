#include "simulator.h"
#include <Arduino.h>

// Private variables
static float currentRPM = 0;
static float targetRPM = 0;
static bool autoMode = false;
static unsigned long lastAutoChange = 0;
static unsigned long autoInterval = 7000;  // 7 detik (lebih lama)

void initSimulator() {
  currentRPM = 0;
  targetRPM = 0;
  autoMode = false;
  lastAutoChange = millis();
}

void updateSimulator() {
  // Auto-simulation
  if (autoMode) {
    unsigned long now = millis();
    if (now - lastAutoChange >= autoInterval) {
      // Random RPM antara 500-2500 (lebih realistis)
      targetRPM = random(500, 2500);
      lastAutoChange = now;
      autoInterval = random(5000, 10000);  // 5-10 detik
    }
  }
  
  // Smooth transition (LEBIH LAMBAT: ±5 RPM per update)
  if (abs(currentRPM - targetRPM) > 5) {
    if (currentRPM < targetRPM) {
      currentRPM += 5;
    } else {
      currentRPM -= 5;
    }
  }
}

float getCurrentRPM() {
  // Tambah noise kecil (±20 RPM) untuk realisme
  float noise = random(-20, 20);
  return currentRPM + noise;
}

void setTargetRPM(float rpm) {
  targetRPM = constrain(rpm, 0, 3000);
  autoMode = false;  // Matikan auto mode
}

bool toggleAutoMode() {
  autoMode = !autoMode;
  return autoMode;
}

bool isAutoMode() {
  return autoMode;
}

void handleCommand(const char* cmd) {
  char buffer[32];
  strncpy(buffer, cmd, 31);
  buffer[31] = '\0';
  
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
    Serial.print(getCurrentRPM());
    Serial.print(" | Target: ");
    Serial.print(targetRPM);
    Serial.print(" | Auto: ");
    Serial.println(autoMode ? "ON" : "OFF");
  }
  else {
    Serial.print("✗ Unknown: ");
    Serial.println(buffer);
    Serial.println("Type 'help' for commands");
  }
}