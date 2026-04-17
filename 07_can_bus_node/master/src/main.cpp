#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

#define CAN_CS_PIN 5
MCP_CAN CAN(CAN_CS_PIN);

void setup() {
  Serial.begin(115200);
  while (!Serial); // Tunggu Serial ready (untuk board tertentu)
  
  Serial.println("\n[MASTER] === START ===");
  
  // 1. INIT CAN (8MHz crystal)
  Serial.print("[MASTER] Initializing CAN... ");
  if (CAN.begin(MCP_STDEXT, CAN_250KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("✅ OK");
  } else {
    Serial.println("❌ FAILED");
    while(1); // Stop jika init gagal
  }
  
  // 2. SET MODE NORMAL
  CAN.setMode(MCP_NORMAL);
  delay(200); // Stabilisasi
  Serial.println("[MASTER] Mode: NORMAL. Waiting ACK from Slave...");
}

void loop() {
  static unsigned long lastTx = 0;
  
  if (millis() - lastTx >= 1000) {
    lastTx = millis();
    
    // Data dummy
    unsigned char data[8] = {0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44};
    
    // Kirim
    byte result = CAN.sendMsgBuf(0x100, 0, 8, data);
    
    if (result == CAN_OK) {
      Serial.println("[MASTER] ✅ TX Success (ACK Received from Slave!)");
    } else {
      Serial.println("[MASTER] ❌ TX Failed (No ACK)");
      // Coba recovery ringan
      CAN.setMode(MCP_NORMAL);
      delay(10);
    }
  }
}