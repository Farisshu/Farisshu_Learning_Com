#include <Arduino.h>
#include <SPI.h>

#define SCK_PIN  18
#define MISO_PIN 19
#define MOSI_PIN 23
#define CS_PIN   25
#define LED1_PIN 2
#define LED2_PIN 14
#define BTN1_PIN 4
#define BTN2_PIN 16

SPIClass spi(VSPI);
unsigned long lastPrint = 0, lastTx = 0;
bool linkOk = false;

byte readBtns() {
  byte s = 0;
  if (digitalRead(BTN1_PIN) == LOW) s |= 0x01;
  if (digitalRead(BTN2_PIN) == LOW) s |= 0x02;
  return s;
}

void setup() {
  Serial.begin(115200);
  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BTN1_PIN, INPUT_PULLUP);
  pinMode(BTN2_PIN, INPUT_PULLUP);
  pinMode(CS_PIN, OUTPUT);
  digitalWrite(CS_PIN, HIGH);

  // 50kHz agar Slave bit-bang stabil di Arduino HAL
  spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
  spi.beginTransaction(SPISettings(50000, MSBFIRST, SPI_MODE0));
  
  Serial.println("🔵 [MASTER] Init 50kHz | Mode 0");
}

void loop() {
  byte local = readBtns();
  digitalWrite(CS_PIN, LOW);
  byte remote = spi.transfer(local);
  digitalWrite(CS_PIN, HIGH);
  lastTx = millis();

  digitalWrite(LED1_PIN, (remote & 0x01) ? HIGH : LOW);
  digitalWrite(LED2_PIN, (remote & 0x02) ? HIGH : LOW);

  if (millis() - lastPrint > 800) {
    lastPrint = millis();
    linkOk = (millis() - lastTx < 1500);
    Serial.printf("[MASTER] Tx:0x%02X Rx:0x%02X | %s\n", 
                  local, remote, linkOk ? "✅ LINK" : "❌ NO LINK");
  }
  delay(15);
}