/**
 * @file main.cpp
 * @brief ESP32 SPI Master-Slave Communication Test
 * @version 1.0
 * @date 2024
 * 
 * This module demonstrates SPI communication between two ESP32 boards
 * using bit-banged slave implementation. Buttons control LEDs on remote board.
 * 
 * @copyright Copyright (c) 2024
 */

#include <Arduino.h>
#include <SPI.h>

/* === Pin Configuration === */
#define SCK_PIN         18                  /**< SPI Clock pin */
#define MISO_PIN        19                  /**< SPI MISO pin */
#define MOSI_PIN        23                  /**< SPI MOSI pin */
#define CS_PIN          25                  /**< SPI Chip Select pin */
#define LED1_PIN        2                   /**< Local LED 1 */
#define LED2_PIN        14                  /**< Local LED 2 */
#define BTN1_PIN        4                   /**< Button 1 (active low) */
#define BTN2_PIN        16                  /**< Button 2 (active low) */

/* === Timing Constants === */
#define TX_INTERVAL_MS  15                  /**< Transmission interval (ms) */
#define LINK_TIMEOUT_MS 1500                /**< Link loss timeout (ms) */
#define PRINT_INTERVAL_MS 800               /**< Serial print interval (ms) */

/* === Hardware Instance === */
static SPIClass spi(VSPI);                  /**< SPI bus instance */

/* === State Variables === */
static unsigned long lastPrintTime = 0;     /**< Last print timestamp */
static unsigned long lastTxTime = 0;        /**< Last transmission timestamp */
static bool linkOk = false;                 /**< Link status flag */

/**
 * @brief Read button states and pack into byte
 * @return Byte with button states (bit 0 = BTN1, bit 1 = BTN2)
 */
static uint8_t readButtons(void) {
    uint8_t state = 0;
    
    if (digitalRead(BTN1_PIN) == LOW) {
        state |= 0x01;
    }
    if (digitalRead(BTN2_PIN) == LOW) {
        state |= 0x02;
    }
    
    return state;
}

/**
 * @brief Initialize SPI and GPIO pins
 */
void setup() {
    Serial.begin(115200);
    
    /* Configure GPIO pins */
    pinMode(LED1_PIN, OUTPUT);
    pinMode(LED2_PIN, OUTPUT);
    pinMode(BTN1_PIN, INPUT_PULLUP);
    pinMode(BTN2_PIN, INPUT_PULLUP);
    pinMode(CS_PIN, OUTPUT);
    digitalWrite(CS_PIN, HIGH);             /**< Deselect slave initially */

    /* Initialize SPI at low speed for bit-banged slave stability */
    spi.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CS_PIN);
    spi.beginTransaction(SPISettings(50000, MSBFIRST, SPI_MODE0));
    
    Serial.println("🔵 [MASTER] Init 50kHz | Mode 0");
}

/**
 * @brief Main loop - Exchange data with slave via SPI
 */
void loop() {
    /* Read local button states */
    const uint8_t localState = readButtons();
    
    /* SPI transaction: send local, receive remote */
    digitalWrite(CS_PIN, LOW);
    const uint8_t remoteState = spi.transfer(localState);
    digitalWrite(CS_PIN, HIGH);
    
    lastTxTime = millis();

    /* Update LEDs based on remote button states */
    digitalWrite(LED1_PIN, (remoteState & 0x01) ? HIGH : LOW);
    digitalWrite(LED2_PIN, (remoteState & 0x02) ? HIGH : LOW);

    /* Periodic status output */
    if (millis() - lastPrintTime > PRINT_INTERVAL_MS) {
        lastPrintTime = millis();
        
        /* Check link status */
        linkOk = (millis() - lastTxTime < LINK_TIMEOUT_MS);
        
        Serial.printf("[MASTER] Tx:0x%02X Rx:0x%02X | %s\n", 
                      localState, remoteState, 
                      linkOk ? "✅ LINK" : "❌ NO LINK");
    }
    
    delay(TX_INTERVAL_MS);
}
