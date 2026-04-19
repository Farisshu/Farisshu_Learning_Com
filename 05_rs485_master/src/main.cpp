/**
 * @file main.cpp
 * @brief RS485 Master Node - Industrial Communication System
 * 
 * This module implements a non-blocking RS485 master node that periodically
 * sends simulated sensor data to slave nodes via UART2.
 * 
 * Features:
 * - Non-blocking timing using millis()
 * - Simulated temperature and pressure sensors
 * - Protocol-based data packet formatting
 * - Cable-based upload and debugging
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#include <Arduino.h>

/* Include global configuration */
#include "config.h"

/* Local module header */
#include "rs485_master.h"

/**
 * @brief Private constants for module operation
 */
static const char* TAG = "[MASTER]";

/**
 * @brief RS485 communication instance
 */
static HardwareSerial RS485(RS485_MASTER_UART_NUM);

/**
 * @brief Timing state variables
 */
static unsigned long lastSendTime = 0;

/**
 * @brief Sensor data state
 */
static SensorData_t sensorData = {
    .temperature = 25.0f,
    .pressure = 1.0f,
    .timestamp = 0,
    .valid = false
};

/**
 * @brief Initialize RS485 communication
 * @return SystemStatus_t Status of RS485 initialization
 */
SystemStatus_t rs485Master_init(void) {
    LOG_INFO(TAG, "Initializing RS485 (UART%d)...", RS485_MASTER_UART_NUM);

    RS485.begin(
        RS485_MASTER_BAUDRATE,
        SERIAL_8N1,
        RS485_MASTER_RX_PIN,
        RS485_MASTER_TX_PIN
    );

    LOG_INFO(TAG, "RS485 initialized at %d baud", RS485_MASTER_BAUDRATE);
    return STATUS_OK;
}

/**
 * @brief Update sensor data with random simulation
 * @param data Pointer to SensorData_t structure
 * @return SystemStatus_t Status of update operation
 */
SystemStatus_t rs485Master_updateData(SensorData_t* data) {
    CHECK_NULL(data, STATUS_ERROR);

    // Simulate sensor value changes with random walk
    data->temperature += (float)random(-15, 20) / 10.0f;
    data->pressure += (float)random(-4, 5) / 10.0f;

    // Constrain values within valid ranges
    data->temperature = constrain(data->temperature, SIM_TEMP_MIN, SIM_TEMP_MAX);
    data->pressure = constrain(data->pressure, SIM_PRESS_MIN, SIM_PRESS_MAX);

    data->timestamp = millis();
    data->valid = true;

    LOG_DEBUG(TAG, "Sensor updated: T=%.1f C, P=%.1f Bar", 
              data->temperature, data->pressure);

    return STATUS_OK;
}

/**
 * @brief Format data packet for transmission
 * @param data Pointer to SensorData_t structure  
 * @param buffer Output buffer for formatted packet
 * @param bufferSize Size of output buffer
 * @return int Length of formatted packet, or -1 on error
 */
int rs485Master_formatPacket(const SensorData_t* data, char* buffer, size_t bufferSize) {
    CHECK_NULL(data, -1);
    CHECK_NULL(buffer, -1);

    if (bufferSize < 50) {
        LOG_ERROR(TAG, "Buffer too small: %d", bufferSize);
        return -1;
    }

    int len = snprintf(buffer, bufferSize, 
                       "DATA|T:%.1f|P:%.1f|END\n",
                       data->temperature,
                       data->pressure);

    if (len < 0 || len >= (int)bufferSize) {
        LOG_ERROR(TAG, "Packet formatting failed");
        return -1;
    }

    return len;
}

/**
 * @brief Send data packet via RS485
 * @param data Pointer to SensorData_t structure
 * @return SystemStatus_t Status of send operation
 */
SystemStatus_t rs485Master_sendData(const SensorData_t* data) {
    CHECK_NULL(data, STATUS_ERROR);

    static char txBuffer[64];

    int packetLen = rs485Master_formatPacket(data, txBuffer, sizeof(txBuffer));
    if (packetLen < 0) {
        return STATUS_ERROR;
    }

    // Send via RS485
    RS485.print(txBuffer);

    // Log transmission
    Serial.print(TAG);
    Serial.print(" [TX] ");
    Serial.print(txBuffer);

    LOG_DEBUG(TAG, "Sent %d bytes via RS485", packetLen);

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
    Serial.println("  RS485 MASTER NODE - Industrial HMI");
    Serial.println("========================================");

    LOG_INFO(TAG, "System booting...");

    /* Initialize RS485 */
    if (rs485Master_init() != STATUS_OK) {
        LOG_ERROR(TAG, "RS485 initialization failed!");
    }

    LOG_INFO(TAG, "System ready. Sending data every %d ms", 
             RS485_MASTER_SEND_INTERVAL_MS);
}

/**
 * @brief Main loop function
 */
void loop() {
    /* Non-blocking timing check */
    unsigned long currentTime = millis();

    if (currentTime - lastSendTime >= RS485_MASTER_SEND_INTERVAL_MS) {
        lastSendTime = currentTime;

        /* Update sensor data */
        if (rs485Master_updateData(&sensorData) == STATUS_OK) {
            /* Send data via RS485 */
            rs485Master_sendData(&sensorData);
        }
    }

    /* Small delay to prevent watchdog trigger */
    delay(1);
}
