/**
 * @file rs485_master.h
 * @brief RS485 Master Node Configuration and Data Structures
 * 
 * @copyright Farisshu Embedded Project
 * @version 1.0.0
 */

#ifndef RS485_MASTER_H
#define RS485_MASTER_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief RS485 Communication Configuration
 */
#define RS485_MASTER_TX_PIN       17
#define RS485_MASTER_RX_PIN       16
#define RS485_MASTER_BAUDRATE     9600
#define RS485_MASTER_UART_NUM     2

/**
 * @brief Timing Configuration
 */
#define RS485_MASTER_SEND_INTERVAL_MS   2000

/**
 * @brief Sensor Data Simulation Range
 */
#define SIM_TEMP_MIN            20.0f
#define SIM_TEMP_MAX            75.0f
#define SIM_PRESS_MIN           0.5f
#define SIM_PRESS_MAX           4.5f

/**
 * @brief Sensor Data Structure
 */
typedef struct {
    float temperature;      /**< Temperature in Celsius */
    float pressure;         /**< Pressure in Bar */
    uint32_t timestamp;     /**< Last update timestamp */
    bool valid;             /**< Data validity flag */
} SensorData_t;

/**
 * @brief Initialize RS485 Master module
 * @return SystemStatus_t Status of initialization
 */
SystemStatus_t rs485Master_init(void);

/**
 * @brief Update sensor data with simulation
 * @param data Pointer to SensorData_t structure
 * @return SystemStatus_t Status of update operation
 */
SystemStatus_t rs485Master_updateData(SensorData_t* data);

/**
 * @brief Send data packet via RS485
 * @param data Pointer to SensorData_t structure
 * @return SystemStatus_t Status of send operation
 */
SystemStatus_t rs485Master_sendData(const SensorData_t* data);

/**
 * @brief Format data packet for transmission
 * @param data Pointer to SensorData_t structure
 * @param buffer Output buffer for formatted packet
 * @param bufferSize Size of output buffer
 * @return int Length of formatted packet
 */
int rs485Master_formatPacket(const SensorData_t* data, char* buffer, size_t bufferSize);

#ifdef __cplusplus
}
#endif

#endif /* RS485_MASTER_H */
