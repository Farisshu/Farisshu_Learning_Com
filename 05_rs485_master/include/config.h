/**
 * @file config.h
 * @brief Global configuration and FreeRTOS settings
 * 
 * This file contains system-wide configurations, logging macros,
 * and FreeRTOS task priorities following FreeRTOS coding standards.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"

// ============================================================================
// VERSION INFORMATION
// ============================================================================
#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    0
#define FW_VERSION_PATCH    0
#define FW_VERSION_STRING   "v1.0.0"

// ============================================================================
// SYSTEM CONFIGURATION
// ============================================================================
#define SYSTEM_TICK_RATE_MS     1000
#define SYSTEM_STACK_SIZE       2048
#define SYSTEM_TASK_PRIORITY    (tskIDLE_PRIORITY + 2)

// ============================================================================
// LOGGING MACROS (FreeRTOS Standard)
// ============================================================================
#ifdef DEBUG
    #define LOG_TAG             "[APP]"
    #define LOG_ERROR(tag, fmt, ...)    Serial.printf("%s [E] " tag ": " fmt "\n", __func__, ##__VA_ARGS__)
    #define LOG_WARN(tag, fmt, ...)     Serial.printf("%s [W] " tag ": " fmt "\n", __func__, ##__VA_ARGS__)
    #define LOG_INFO(tag, fmt, ...)     Serial.printf("%s [I] " tag ": " fmt "\n", __func__, ##__VA_ARGS__)
    #define LOG_DEBUG(tag, fmt, ...)    Serial.printf("%s [D] " tag ": " fmt "\n", __func__, ##__VA_ARGS__)
#else
    #define LOG_ERROR(tag, fmt, ...)    Serial.printf("[E] " tag ": " fmt "\n", ##__VA_ARGS__)
    #define LOG_WARN(tag, fmt, ...)     // Disabled in release
    #define LOG_INFO(tag, fmt, ...)     Serial.printf("[I] " tag ": " fmt "\n", ##__VA_ARGS__)
    #define LOG_DEBUG(tag, fmt, ...)    // Disabled in release
#endif

// ============================================================================
// ERROR HANDLING MACROS
// ============================================================================
#define CHECK_NULL(ptr, retVal) \
    do { \
        if ((ptr) == NULL) { \
            LOG_ERROR("NULL_CHECK", "NULL pointer detected"); \
            return (retVal); \
        } \
    } while(0)

#define CHECK_ESP_ERR(err, retVal) \
    do { \
        if ((err) != ESP_OK) { \
            LOG_ERROR("ESP_ERR", "ESP error: %d", (int)(err)); \
            return (retVal); \
        } \
    } while(0)

// ============================================================================
// FREERTOS UTILITIES
// ============================================================================
#define TASK_DELAY_MS(ms)       vTaskDelay(pdMS_TO_TICKS(ms))
#define TASK_DELAY_UNTIL(tick, delay_ms)  vTaskDelayUntil(&(tick), pdMS_TO_TICKS(delay_ms))

// Stack size definitions (in words, typically 4 bytes per word on ESP32)
#define STACK_SMALL     1024
#define STACK_MEDIUM    2048
#define STACK_LARGE     4096

// Task priorities (higher number = higher priority)
#define PRIORITY_LOW    (tskIDLE_PRIORITY + 1)
#define PRIORITY_NORMAL (tskIDLE_PRIORITY + 2)
#define PRIORITY_HIGH   (tskIDLE_PRIORITY + 3)
#define PRIORITY_CRITICAL (tskIDLE_PRIORITY + 4)

// ============================================================================
// TYPE DEFINITIONS (FreeRTOS Standard - PascalCase for types)
// ============================================================================
typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR = -1,
    STATUS_TIMEOUT = -2,
    STATUS_INVALID_PARAM = -3,
    STATUS_NOT_INITIALIZED = -4
} SystemStatus_t;

typedef enum {
    MODE_MANUAL = 0,
    MODE_AUTO = 1,
    MODE_TEST = 2
} OperationMode_t;

// ============================================================================
// ASSERT MACRO (for development)
// ============================================================================
#ifdef DEBUG
    #define ASSERT(condition, message) \
        do { \
            if (!(condition)) { \
                LOG_ERROR("ASSERT", "Failed: %s at %s:%d", message, __FILE__, __LINE__); \
                vTaskSuspendAll(); \
                while(1) { ; } \
            } \
        } while(0)
#else
    #define ASSERT(condition, message)    // Disabled in release
#endif

#endif /* CONFIG_H */
