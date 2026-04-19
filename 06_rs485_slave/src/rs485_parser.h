/**
 * @file rs485_parser.h
 * @brief RS485 Protocol Parser for Slave Node
 * 
 * This module handles parsing of incoming RS485 data packets
 * from the master node in industrial communication systems.
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#ifndef RS485_PARSER_H
#define RS485_PARSER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RS485 Data Structure
 */
typedef struct {
    float temperature;      /**< Temperature value in Celsius */
    float pressure;         /**< Pressure value in Bar */
    bool valid;             /**< Data validity flag */
} Rs485Data_t;

/**
 * @brief Initialize RS485 communication
 * @param baud Baud rate for RS485 communication
 * @param rxPin RX pin number
 * @param txPin TX pin number
 */
void rs485_init(uint32_t baud, int8_t rxPin, int8_t txPin);

/**
 * @brief Poll RS485 for incoming data (non-blocking)
 * @param outData Pointer to output data structure
 */
void rs485_poll(Rs485Data_t* outData);

/**
 * @brief Check if RS485 link is alive
 * @param timeoutMs Timeout threshold in milliseconds
 * @return true if link is active, false otherwise
 */
bool rs485_isLinkAlive(uint32_t timeoutMs);

#ifdef __cplusplus
}
#endif

#endif /* RS485_PARSER_H */