#include "rs485_parser.h"

static HardwareSerial RS485(2);
static char rxBuffer[64];
static uint8_t bufIdx = 0;
static unsigned long lastValidRx = 0;

void rs485_init(uint32_t baud, int8_t rxPin, int8_t txPin) {
  RS485.begin(baud, SERIAL_8N1, rxPin, txPin);
}

void rs485_poll(Rs485Data_t* outData) {
  outData->valid = false;
  while (RS485.available()) {
    char c = RS485.read();
    // Filter karakter printable (anti noise floating line)
    if (c >= 32 && c <= 126) {
      if (bufIdx < 63) rxBuffer[bufIdx++] = c;
    }
    if (c == '\n' || c == '\r') {
      rxBuffer[bufIdx] = '\0';
      // Validasi header protokol
      if (bufIdx > 5 && strncmp(rxBuffer, "DATA|", 5) == 0) {
        if (sscanf(rxBuffer, "DATA|T:%f|P:%f|END", &outData->temp, &outData->pressure) == 2) {
          outData->valid = true;
          lastValidRx = millis();
        }
      }
      bufIdx = 0;
    }
  }
}

bool rs485_isLinkAlive(uint32_t timeoutMs) {
  return (millis() - lastValidRx < timeoutMs);
}