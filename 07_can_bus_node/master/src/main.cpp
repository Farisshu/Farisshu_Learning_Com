#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS_PIN 5
MCP_CAN CAN(CAN_CS_PIN);

// Variabel simulasi sensor
float simRPM = 800.0;
float simTemp = 85.0;
uint8_t simStatus = 0x00;

void setup() {
  Serial.begin(115200);
  while(!Serial);
  Serial.println("[MASTER] Init CAN...");
  
  while (CAN.begin(MCP_STDEXT, CAN_250KBPS, MCP_8MHZ) != CAN_OK) {
    Serial.println("[MASTER] ❌ Init failed");
    delay(500);
  }
  CAN.setMode(MCP_NORMAL);
  delay(100);
  randomSeed(analogRead(0)); // Agar simulasi tidak statis
  Serial.println("[MASTER] ✅ CAN Ready. TX setiap 1s.");
}

void loop() {
  static unsigned long lastTx = 0;
  if (millis() - lastTx >= 1000) {
    lastTx = millis();

    // Simulasi perubahan nilai realistis
    simRPM = constrain(simRPM + random(-150, 150), 700, 6000);
    simTemp = constrain(simTemp + random(-5, 6) / 10.0, 75.0, 105.0);
    simStatus = (simTemp > 100.0) ? 0x02 : (simTemp > 95.0) ? 0x01 : 0x00;

    // Packing data (Big-Endian, skala x10)
    uint16_t rpmRaw = (uint16_t)(simRPM * 10);
    uint16_t tempRaw = (uint16_t)(simTemp * 10);
    unsigned char data[8] = {
      (rpmRaw >> 8) & 0xFF,
      rpmRaw & 0xFF,
      (tempRaw >> 8) & 0xFF,
      tempRaw & 0xFF,
      simStatus,
      0x00, 0x00, 0x00
    };

    if (CAN.sendMsgBuf(0x100, 0, 8, data) == CAN_OK) {
      Serial.printf("[TX] ✅ ID:0x100 | RPM:%.0f | TEMP:%.1fC | ST:0x%02X\n", 
                    simRPM, simTemp, simStatus);
    }
  }
}