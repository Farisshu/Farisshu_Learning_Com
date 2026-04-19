/**
 * @file main.cpp
 * @brief CAN Bus Master Node - Simulates sensor data transmission
 * @version 1.0
 * @date 2024
 * 
 * This module simulates a CAN bus master node that transmits
 * RPM, temperature, and status data every 1 second.
 * 
 * @copyright Copyright (c) 2024
 */

#include <Arduino.h>
#include "config.h"
#include <SPI.h>
#include <mcp_can.h>

/* === Configuration Constants === */
#define CAN_CS_PIN        5                   /**< Chip Select pin for MCP2515 */
#define SIM_RPM_MIN       700                 /**< Minimum simulated RPM */
#define SIM_RPM_MAX       6000                /**< Maximum simulated RPM */
#define SIM_TEMP_MIN      75.0f               /**< Minimum simulated temperature (°C) */
#define SIM_TEMP_MAX      105.0f              /**< Maximum simulated temperature (°C) */
#define TX_INTERVAL_MS    1000UL              /**< Transmission interval in milliseconds */
#define CAN_MSG_ID        0x100UL             /**< CAN message ID */

/* === Hardware Instance === */
static MCP_CAN CAN(CAN_CS_PIN);               /**< MCP2515 CAN controller instance */

/* === Simulation State === */
static float simRpm = 800.0f;                 /**< Current simulated RPM */
static float simTemp = 85.0f;                 /**< Current simulated temperature */
static uint8_t simStatus = 0x00;              /**< Current system status */

/**
 * @brief Initialize CAN bus and serial communication
 */
void setup() {
    Serial.begin(115200);
    while (!Serial);                          /**< Wait for serial port */
    
    Serial.println("[MASTER] Init CAN...");
    
    /* Initialize CAN controller with retry loop */
    while (CAN.begin(MCP_STDEXT, CAN_250KBPS, MCP_8MHZ) != CAN_OK) {
        Serial.println("[MASTER] ❌ Init failed");
        delay(500);
    }
    
    CAN.setMode(MCP_NORMAL);
    delay(100);
    
    /* Initialize random seed for realistic simulation */
    randomSeed(analogRead(0));
    
    Serial.println("[MASTER] ✅ CAN Ready. TX every 1s.");
}

/**
 * @brief Main loop - Transmit sensor data periodically
 */
void loop() {
    static unsigned long lastTx = 0;
    
    if (millis() - lastTx >= TX_INTERVAL_MS) {
        lastTx = millis();

        /* Simulate realistic value changes */
        simRpm = constrain(simRpm + random(-150, 150), SIM_RPM_MIN, SIM_RPM_MAX);
        simTemp = constrain(simTemp + random(-5, 6) / 10.0f, SIM_TEMP_MIN, SIM_TEMP_MAX);
        
        /* Update status based on temperature */
        if (simTemp > 100.0f) {
            simStatus = 0x02;                 /**< Critical */
        } else if (simTemp > 95.0f) {
            simStatus = 0x01;                 /**< Warning */
        } else {
            simStatus = 0x00;                 /**< Normal */
        }

        /* Pack data (Big-Endian, scaled x10) */
        const uint16_t rpmRaw = (uint16_t)(simRpm * 10.0f);
        const uint16_t tempRaw = (uint16_t)(simTemp * 10.0f);
        
        const uint8_t txData[8] = {
            static_cast<uint8_t>((rpmRaw >> 8) & 0xFF),
            static_cast<uint8_t>(rpmRaw & 0xFF),
            static_cast<uint8_t>((tempRaw >> 8) & 0xFF),
            static_cast<uint8_t>(tempRaw & 0xFF),
            simStatus,
            0x00, 0x00, 0x00                  /**< Reserved bytes */
        };

        /* Transmit CAN message */
        if (CAN.sendMsgBuf(CAN_MSG_ID, 0, 8, const_cast<uint8_t*>(txData)) == CAN_OK) {
            Serial.printf("[TX] ✅ ID:0x%03lX | RPM:%6.0f | TEMP:%6.1f°C | ST:0x%02X\n", 
                          CAN_MSG_ID, simRpm, simTemp, simStatus);
        } else {
            Serial.println("[TX] ❌ Transmission failed");
        }
    }
}