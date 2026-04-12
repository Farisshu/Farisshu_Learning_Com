#ifndef LOGIC_H
#define LOGIC_H

struct SensorData {
    float temp;
    float pressure;
    bool alarm;
};

void initLogic();
void updateSensorData();
bool checkAlarmStatus(float limitTemp, float limitPressure);
void handleLogicCommand(const char* cmd, SensorData* data);
SensorData* getData(); // ✅ Wajib ada ini

#endif