#include "logic.h"
#include <Arduino.h>

static SensorData currentData = {25.0, 1.0, false};
static unsigned long lastUpdate = 0;

void initLogic() {
    currentData.temp = 25.0;
    currentData.pressure = 1.0;
    currentData.alarm = false;
}

void updateSensorData() {
    // Update tiap 100ms
    if (millis() - lastUpdate < 100) return;
    lastUpdate = millis();

    // Simulasi perubahan acak (Random Walk)
    currentData.temp += random(-30, 30) / 10.0;
    currentData.pressure += random(-5, 5) / 10.0;

    // Batas minimum agar tidak minus (fisik)
    if (currentData.temp < 20.0) currentData.temp = 20.0;
    if (currentData.pressure < 0.5) currentData.pressure = 0.5;
}

bool checkAlarmStatus(float limitTemp, float limitPressure) {
    if (currentData.temp > limitTemp || currentData.pressure > limitPressure) {
        currentData.alarm = true;
        return true;
    }
    currentData.alarm = false;
    return false;
}

// Mengambil data untuk ditampilkan
SensorData* getData() {
    return &currentData;
}

void handleLogicCommand(const char* cmd, SensorData* data) {
    char buffer[32];
    strncpy(buffer, cmd, 31);
    buffer[31] = '\0';

    if (strncmp(buffer, "temp ", 5) == 0) {
        data->temp = atof(buffer + 5);
        Serial.print("Simulasi Suhu diset ke: "); Serial.println(data->temp);
    } else if (strncmp(buffer, "press ", 6) == 0) {
        data->pressure = atof(buffer + 6);
        Serial.print("Simulasi Tekanan diset ke: "); Serial.println(data->pressure);
    } else {
        Serial.println("Cmd Logic: temp <val>, press <val>");
    }
}