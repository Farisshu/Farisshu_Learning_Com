#include <Arduino.h>
#include "ui.h"
#include "logic.h"

#define MAX_TEMP 60.0
#define MAX_PRESS 5.0

char buffer[32] = "";
int idx = 0;

void setup() {
    Serial.begin(115200);
    initUI();
    initLogic();
    Serial.println("✅ Industrial HMI Ready. Ketik 'temp 70' untuk tes alarm.");
}

void loop() {
    // 1. Update Data Sensor
    updateSensorData();
    SensorData* data = getData();
    bool isAlarm = checkAlarmStatus(MAX_TEMP, MAX_PRESS);

    // 2. Baca Serial (Anti-Spam)
    while (Serial.available() > 0) {
        char c = Serial.read();
        if (c == '\n' || c == '\r') {
            if (idx > 0) { // ✅ Hanya proses jika ada isi
                buffer[idx] = 0;
                handleLogicCommand(buffer, data);
            }
            idx = 0;
        } else {
            if (idx < 31) buffer[idx++] = c;
        }
    }

    // 3. Render UI (Non-Blocking Timer)
    static unsigned long lastDraw = 0;
    if (millis() - lastDraw >= 100) { // Update tiap 100ms
        if (isAlarm) {
            renderAlarm();
        } else {
            renderDashboard(data);
        }
        lastDraw = millis();
    }
}