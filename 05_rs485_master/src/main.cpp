/**
 * @file main.cpp
 * @brief RS485 Master Node - Industrial Communication System
 * 
 * This module implements a non-blocking RS485 master node that periodically
 * sends simulated sensor data to slave nodes via UART2.
 * 
 * Features:
 * - Non-blocking timing using millis()
 * - WiFi connectivity with OTA updates
 * - Simulated temperature and pressure sensors
 * - Protocol-based data packet formatting
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

/* Include global configuration */
#include "config.h"

/* Local module header */
#include "rs485_master.h"

#ifdef SECRETS_H_AVAILABLE
#include "secrets.h"
#else
#warning "secrets.h not found. Please create secrets.h with WIFI_SSID, WIFI_PASS, and OTA_PASS"
#define WIFI_SSID "YOUR_WIFI_SSID"
#define WIFI_PASS "YOUR_WIFI_PASSWORD"
#define OTA_PASS "YOUR_OTA_PASSWORD"
#endif

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
 * @brief Initialize WiFi connection
 * @return SystemStatus_t Status of WiFi initialization
 */
static SystemStatus_t wifi_init(void) {
    LOG_INFO(TAG, "Initializing WiFi...");
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    
    Serial.print(TAG);
    Serial.print(" WiFi connecting");
    
    int attempts = 0;
    const int maxAttempts = 20; // 10 seconds timeout
    
    while (WiFi.status() != WL_CONNECTED && attempts < maxAttempts) {
        delay(500);
        Serial.print(".");
        attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("");
        LOG_INFO(TAG, "WiFi Connected. IP: %s", WiFi.localIP().toString().c_str());
        return STATUS_OK;
    } else {
        Serial.println("");
        LOG_ERROR(TAG, "WiFi connection failed after %d attempts", attempts);
        return STATUS_ERROR;
    }
}

/**
 * @brief Initialize OTA updates
 * @return SystemStatus_t Status of OTA initialization
 */
static SystemStatus_t ota_init(void) {
    LOG_INFO(TAG, "Initializing OTA...");
    
    ArduinoOTA.setHostname("ESP32-MASTER");
    ArduinoOTA.setPassword(OTA_PASS);
    
    ArduinoOTA.onStart([]() {
        LOG_INFO(TAG, "OTA Update started");
    });
    
    ArduinoOTA.onEnd([]() {
        LOG_INFO(TAG, "OTA Update completed");
    });
    
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        LOG_INFO(TAG, "OTA Progress: %u%%", (progress / (total / 100)));
    });
    
    ArduinoOTA.onError([](ota_error_t error) {
        LOG_ERROR(TAG, "OTA Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) LOG_ERROR(TAG, "Auth Failed");
        else if (error == OTA_BEGIN_ERROR) LOG_ERROR(TAG, "Begin Failed");
        else if (error == OTA_CONNECT_ERROR) LOG_ERROR(TAG, "Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) LOG_ERROR(TAG, "Receive Failed");
        else if (error == OTA_END_ERROR) LOG_ERROR(TAG, "End Failed");
    });
    
    ArduinoOTA.begin();
    LOG_INFO(TAG, "OTA initialized");
    
    return STATUS_OK;
}

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
    
    /* Initialize WiFi */
    if (wifi_init() != STATUS_OK) {
        LOG_WARN(TAG, "Continuing without WiFi...");
    }
    
    /* Initialize OTA */
    if (ota_init() != STATUS_OK) {
        LOG_WARN(TAG, "OTA initialization failed!");
    }
    
    LOG_INFO(TAG, "System ready. Sending data every %d ms", 
             RS485_MASTER_SEND_INTERVAL_MS);
}

/**
 * @brief Main loop function
 */
void loop() {
    /* Handle OTA updates */
    ArduinoOTA.handle();
    
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