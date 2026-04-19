/**
 * @file rs485_parser.cpp
 * @brief RS485 Protocol Parser Implementation
 * 
 * Implements non-blocking parsing of RS485 data packets
 * with noise filtering and protocol validation.
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#include "rs485_parser.h"

/* Include global configuration */
#include "config.h"

/* HardwareSerial include for ESP32 */
#include <HardwareSerial.h>

/**
 * @brief Private constants
 */
#define RS485_BUFFER_SIZE       64
#define RS485_UART_NUM          2
#define PROTOCOL_HEADER         "DATA|"
#define PROTOCOL_HEADER_LEN     5

/**
 * @brief Private variables
 */
static HardwareSerial RS485(RS485_UART_NUM);
static char rxBuffer[RS485_BUFFER_SIZE];
static uint8_t bufIdx = 0;
static unsigned long lastValidRx = 0;
static portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

/**
 * @brief Validate and parse incoming packet
 * @param packet Null-terminated packet string
 * @param data Pointer to output data structure
 * @return true if parsing successful, false otherwise
 */
static bool parsePacket(const char* packet, Rs485Data_t* data) {
    CHECK_NULL(packet, false);
    CHECK_NULL(data, false);
    
    /* Validate protocol header */
    if (strncmp(packet, PROTOCOL_HEADER, PROTOCOL_HEADER_LEN) != 0) {
        return false;
    }
    
    /* Parse temperature and pressure values */
    float temp = 0.0f;
    float pressure = 0.0f;
    
    int parsed = sscanf(packet, "DATA|T:%f|P:%f|END", &temp, &pressure);
    
    if (parsed == 2) {
        data->temperature = temp;
        data->pressure = pressure;
        return true;
    }
    
    return false;
}

/**
 * @brief Initialize RS485 communication
 */
void rs485_init(uint32_t baud, int8_t rxPin, int8_t txPin) {
    RS485.begin(baud, SERIAL_8N1, rxPin, txPin);
}

/**
 * @brief Poll RS485 for incoming data (non-blocking)
 */
void rs485_poll(Rs485Data_t* outData) {
    if (outData == NULL) {
        return;
    }
    
    outData->valid = false;
    
    while (RS485.available()) {
        char c = RS485.read();
        
        /* Filter printable characters only (noise rejection) */
        if (c >= 32 && c <= 126) {
            if (bufIdx < (RS485_BUFFER_SIZE - 1)) {
                rxBuffer[bufIdx++] = c;
            } else {
                // Buffer overflow - reset to prevent lockout
                bufIdx = 0;
                LOG_WARN("RS485", "Buffer overflow, resetting");
            }
        }
        
        /* Check for line terminator */
        if (c == '\n' || c == '\r') {
            rxBuffer[bufIdx] = '\0';
            
            /* Validate and parse packet */
            if (bufIdx > PROTOCOL_HEADER_LEN) {
                if (parsePacket(rxBuffer, outData)) {
                    outData->valid = true;
                    /* Atomic write to lastValidRx */
                    portENTER_CRITICAL_SAFE(&mux);
                    lastValidRx = millis();
                    portEXIT_CRITICAL_SAFE(&mux);
                } else {
                    LOG_DEBUG("RS485", "Parse failed: %s", rxBuffer);
                }
            }
            
            /* Reset buffer index */
            bufIdx = 0;
        }
    }
}

/**
 * @brief Check if RS485 link is alive
 */
bool rs485_isLinkAlive(uint32_t timeoutMs) {
    /* Atomic read of lastValidRx */
    portENTER_CRITICAL_SAFE(&mux);
    unsigned long lastRx = lastValidRx;
    portEXIT_CRITICAL_SAFE(&mux);
    
    return (millis() - lastRx < timeoutMs);
}