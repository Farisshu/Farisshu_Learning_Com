#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "secrets.h"

// === RS485 CONFIG (UART2) ===
#define RS485_TX 17
#define RS485_RX 16
#define RS485_BAUD 9600
HardwareSerial RS485(2); // UART2

// === NON-BLOCKING TIMER ===
unsigned long lastSend = 0;
const unsigned long INTERVAL = 2000; // Kirim tiap 2 detik

// === SIMULASI DATA ===
float simTemp = 25.0;
float simPress = 1.0;

void setup() {
  Serial.begin(115200);
  Serial.println("[MASTER] Booting...");

  // Init RS485
  RS485.begin(RS485_BAUD, SERIAL_8N1, RS485_RX, RS485_TX);

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[MASTER] WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n[MASTER] IP: " + WiFi.localIP().toString());

  // OTA
  ArduinoOTA.setHostname("ESP32-MASTER");
  ArduinoOTA.setPassword(OTA_PASS);
  ArduinoOTA.begin();

  Serial.println("[MASTER] Ready. Data akan dikirim via RS485.");
}

void loop() {
  ArduinoOTA.handle();

  unsigned long now = millis();
  if (now - lastSend >= INTERVAL) {
    lastSend = now;

    // Simulasi perubahan sensor
    simTemp += random(-15, 20) / 10.0;
    simPress += random(-4, 5) / 10.0;
    simTemp = constrain(simTemp, 20.0, 75.0);
    simPress = constrain(simPress, 0.5, 4.5);

    // Format paket data (protocol sederhana)
    String packet = String("DATA|T:") + simTemp + "|P:" + simPress + "|END\n";
    
    // Kirim ke RS485
    RS485.print(packet);
    Serial.print("[TX] ");
    Serial.println(packet);
  }
}