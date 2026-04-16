#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS_PIN 22  // Sesuaikan dengan wiring Slave (5 atau 22)
MCP_CAN CAN(CAN_CS_PIN);

unsigned long lastRxTime = 0;
const unsigned long TIMEOUT_MS = 3000;

void setup() {
  Serial.begin(115200);
  while (!Serial); // Wait for serial port (important for some boards)
  
  Serial.println("\n[SLAVE] === START ===");
  
  // 1. INIT CAN (8MHz crystal - sesuai foto modulmu)
  Serial.print("[SLAVE] Initializing CAN... ");
  if (CAN.begin(MCP_STDEXT, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("✅ OK");
  } else {
    Serial.println("❌ FAILED - Cek SPI wiring & power");
    while(1); // Halt jika init gagal
  }
  
  // 2. SET MODE NORMAL (WAJIB untuk terima data dari node lain)
  CAN.setMode(MCP_NORMAL);
  delay(200); // Stabilisasi transceiver TJA1050
  Serial.println("[SLAVE] Mode: NORMAL. Waiting for ID:0x100...");
  Serial.println("[SLAVE] Jika terima data, akan muncul: [RX] ✅ ID:0x100 ...");
}

void loop() {
  // Cek buffer CAN
  if (CAN.checkReceive() == CAN_MSGAVAIL) {
    unsigned long canId;
    unsigned char len;
    unsigned char buf[8];
    
    if (CAN.readMsgBuf(&canId, &len, buf) == CAN_OK) {
      // Filter hanya ID 0x100 dari Master
      if (canId == 0x100) {
        lastRxTime = millis();
        
        // Parse data (sesuai format Master)
        uint16_t rpmRaw = (buf[0] << 8) | buf[1];
        uint16_t tempRaw = (buf[2] << 8) | buf[3];
        float rpm = rpmRaw / 10.0;
        float temp = tempRaw / 10.0;
        uint8_t status = buf[4];
        
        // Output debug yang jelas
        Serial.printf("[RX] ✅ ID:0x%03X | RPM:%.0f | TEMP:%.1fC | STATUS:0x%02X\n", 
                      canId, rpm, temp, status);
        Serial.printf("      Raw: %02X %02X %02X %02X %02X %02X %02X %02X\n",
                      buf[0], buf[1], buf[2], buf[3], buf[4], buf[5], buf[6], buf[7]);
      } else {
        // Jika terima ID lain (noise atau device lain)
        Serial.printf("[RX] ⚠️ ID:0x%03X (bukan 0x100)\n", canId);
      }
    }
  }
  
  // Timeout detection (link lost)
  if (lastRxTime > 0 && millis() - lastRxTime > TIMEOUT_MS) {
    Serial.println("[SLAVE] ⚠️ LINK LOST - No data from Master >3s");
    lastRxTime = 0; // Reset flag
  }
  
  delay(50); // Small yield untuk stabilitas
}