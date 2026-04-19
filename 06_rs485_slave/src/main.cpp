/**
 * @file main.cpp
 * @brief RS485 Slave Node - Industrial Communication System
 * 
 * This module implements an RS485 slave node that receives sensor data
 * from a master node and displays it on a TFT screen.
 * 
 * Features:
 * - Non-blocking RS485 data polling
 * - Real-time TFT display updates
 * - Hardware watchdog timer for system reliability
 * - Link status monitoring with timeout detection
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#include <Arduino.h>
#include "esp_task_wdt.h"

/* Include global configuration */
#include "config.h"

/* Local module headers */
#include "ui_display.h"
#include "rs485_parser.h"

/**
 * @brief Private constants for module operation
 */
#define RS485_SLAVE_BAUDRATE      9600
#define RS485_SLAVE_RX_PIN        16
#define RS485_SLAVE_TX_PIN        17
#define LINK_TIMEOUT_MS           5000
#define UI_UPDATE_INTERVAL_MS     400
#define WATCHDOG_TIMEOUT_S        8

/**
 * @brief Tag for logging
 */
static const char* TAG = "[SLAVE]";

/**
 * @brief Sensor data state
 */
static Rs485Data_t sensorData = {
    .temperature = 0.0f,
    .pressure = 0.0f,
    .valid = false
};

/**
 * @brief UI update timing state
 */
static unsigned long lastUiUpdateTime = 0;

/**
 * @brief Initialize hardware watchdog timer
 * @return SystemStatus_t Status of WDT initialization
 */
static SystemStatus_t wdt_init(void) {
    LOG_INFO(TAG, "Initializing Watchdog Timer (%ds)...", WATCHDOG_TIMEOUT_S);
    
    esp_task_wdt_init(WATCHDOG_TIMEOUT_S, true);
    esp_task_wdt_add(NULL);
    
    return STATUS_OK;
}

/**
 * @brief Main setup function
 */
void setup() {
    /* Initialize Serial for debugging */
    Serial.begin(115200);
    delay(100); // Wait for serial to stabilize
    
    Serial.println("");
    Serial.println("========================================");
    Serial.println("  RS485 SLAVE NODE - Industrial HMI");
    Serial.println("========================================");
    
    LOG_INFO(TAG, "System initializing...");
    
    /* Initialize Watchdog Timer */
    if (wdt_init() != STATUS_OK) {
        LOG_ERROR(TAG, "WDT initialization failed!");
    }
    
    /* Initialize RS485 communication */
    rs485_init(RS485_SLAVE_BAUDRATE, RS485_SLAVE_RX_PIN, RS485_SLAVE_TX_PIN);
    LOG_INFO(TAG, "RS485 initialized at %d baud", RS485_SLAVE_BAUDRATE);
    
    /* Initialize display */
    ui_init();
    LOG_INFO(TAG, "Display initialized");
    
    LOG_INFO(TAG, "System ready. Waiting for master data...");
}

/**
 * @brief Main loop function
 */
void loop() {
    /* 1. Poll RS485 for incoming data (non-blocking) */
    rs485_poll(&sensorData);
    
    /* 2. Update UI at fixed interval (non-blocking) */
    unsigned long currentTime = millis();
    
    if (currentTime - lastUiUpdateTime >= UI_UPDATE_INTERVAL_MS) {
        lastUiUpdateTime = currentTime;
        
        /* Check link status */
        bool linkActive = rs485_isLinkAlive(LINK_TIMEOUT_MS);
        
        /* Update display with current values */
        ui_update(sensorData.temperature, sensorData.pressure, linkActive);
        
        /* Log link status changes */
        static bool previousLinkState = false;
        if (linkActive != previousLinkState) {
            if (linkActive) {
                LOG_INFO(TAG, "Link established with master");
            } else {
                LOG_WARN(TAG, "Link lost - timeout > %d ms", LINK_TIMEOUT_MS);
            }
            previousLinkState = linkActive;
        }
    }
    
    /* 3. Reset Watchdog Timer (required in main loop) */
    esp_task_wdt_reset();
    
    /* Small yield for RTOS background tasks */
    delay(1);
}