#include <Arduino.h>
#include "esp_task_wdt.h"
#include "ui_display.h"
#include "rs485_parser.h"

constexpr uint32_t RS485_BAUD = 9600;
constexpr int8_t   RS485_RX   = 16;
constexpr int8_t   RS485_TX   = 17;
constexpr uint32_t LINK_TIMEOUT_MS = 5000;
constexpr uint32_t UI_UPDATE_MS    = 400;

static Rs485Data_t sensorData = {0.0f, 0.0f, false};
static unsigned long lastUiUpdate = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("[SLAVE] Initializing...");

  // Hardware Watchdog (8 detik default ESP32)
  esp_task_wdt_init(8, true);
  esp_task_wdt_add(NULL);

  rs485_init(RS485_BAUD, RS485_RX, RS485_TX);
  ui_init();

  Serial.println("[SLAVE] Ready.");
}

void loop() {
  // 1. Poll RS485 (non-blocking)
  rs485_poll(&sensorData);

  // 2. Update UI (non-blocking, fixed interval)
  unsigned long now = millis();
  if (now - lastUiUpdate >= UI_UPDATE_MS) {
    lastUiUpdate = now;
    bool linkOk = rs485_isLinkAlive(LINK_TIMEOUT_MS);
    ui_update(sensorData.temp, sensorData.pressure, linkOk);
  }

  // 3. Reset Watchdog (wajib di loop utama)
  esp_task_wdt_reset();
  
  // Yield kecil agar RTOS background tasks jalan
  delay(1);
}