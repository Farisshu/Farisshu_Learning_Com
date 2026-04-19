/**
 * @file can_common.h
 * @brief Common definitions and types for CAN Bus communication
 * @version 1.0
 * @date 2024
 * 
 * @copyright Copyright (c) 2024
 */

#ifndef _CAN_COMMON_H_
#define _CAN_COMMON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief CAN Message ID
 */
#define CAN_MSG_ID          0x100UL

/**
 * @brief CAN Bus Baudrate
 */
#define CAN_BAUDRATE        CAN_250KBPS

/**
 * @brief CAN Oscillator Frequency
 */
#define CAN_OSC_FREQ        MCP_8MHZ

/**
 * @brief Data packet structure for CAN communication
 */
typedef struct {
    uint16_t rpm_raw;       /**< RPM value (scaled x10) */
    uint16_t temp_raw;      /**< Temperature value (scaled x10) */
    uint8_t  status;        /**< System status flags */
    uint32_t timestamp;     /**< Timestamp in milliseconds */
} CanDataPacket_t;

/**
 * @brief System status flags
 */
typedef enum {
    STATUS_NORMAL = 0x00,   /**< Normal operation */
    STATUS_WARNING = 0x01,  /**< Temperature warning (>95°C) */
    STATUS_CRITICAL = 0x02  /**< Temperature critical (>100°C) */
} SystemStatusFlag_t;

#ifdef __cplusplus
}
#endif

#endif /* _CAN_COMMON_H_ */
