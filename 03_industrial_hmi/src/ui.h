#ifndef UI_H
#define UI_H

#include <TFT_eSPI.h>
#include "logic.h"

void initUI();
void renderDashboard(SensorData* data);
void renderAlarm(); // ✅ Hapus parameter data (tidak dipakai)

#endif