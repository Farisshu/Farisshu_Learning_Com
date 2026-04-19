/**
 * @file main.cpp
 * @brief CAN Bus Slave Node - Receives and monitors sensor data using FreeRTOS
 * @version 1.0
 * @date 2024
 * 
 * This module implements a CAN bus slave node that receives sensor data
 * from the master node and monitors for link loss conditions.
 * Uses FreeRTOS tasks and queues for real-time operation.
 * 
 * @copyright Copyright (c) 2024
 */

#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

/* === FreeRTOS Headers === */
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

/* === Configuration Constants === */
#define CAN_CS_PIN          22                  /**< Chip Select pin for MCP2515 */
#define CAN_BAUDRATE        CAN_250KBPS         /**< CAN bus baudrate */
#define CAN_OSC_FREQ        MCP_8MHZ            /**< CAN oscillator frequency */
#define CAN_MSG_ID          0x100UL             /**< Expected CAN message ID */
#define QUEUE_SIZE          5                   /**< Queue capacity */
#define TIMEOUT_MS          3000UL              /**< Link loss timeout (ms) */
#define TASK_STACK_SIZE     4096                /**< Task stack size in words */
#define TASK_PRIORITY_RX    2                   /**< CAN RX task priority */
#define TASK_PRIORITY_MON   1                   /**< Monitor task priority */

/* === Hardware Instance === */
static MCP_CAN CAN(CAN_CS_PIN);                 /**< MCP2515 CAN controller */

/* === Data Structures === */
/**
 * @brief CAN data packet structure
 */
typedef struct {
    uint16_t rpmRaw;        /**< RPM value (scaled x10) */
    uint16_t tempRaw;       /**< Temperature value (scaled x10) */
    uint8_t  status;        /**< System status flags */
    uint32_t timestamp;     /**< Reception timestamp in ms */
} CanDataPacket_t;

/* === Global Handles === */
static QueueHandle_t qCanData = NULL;           /**< Queue for CAN data */

/**
 * @brief Task: CAN Receiver (High Priority)
 * 
 * Continuously monitors CAN bus for incoming messages and forwards
 * valid data to the monitor task via queue.
 */
static void taskCanRx(void *pvParameters) {
    (void)pvParameters;                         /**< Suppress unused parameter warning */

    Serial.println("[RX] Initializing CAN Controller...");
    
    /* Initialize CAN with retry loop */
    while (CAN.begin(MCP_STDEXT, CAN_BAUDRATE, CAN_OSC_FREQ) != CAN_OK) {
        Serial.println("[RX] ❌ Init Failed. Retrying in 1s...");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    CAN.setMode(MCP_NORMAL);
    Serial.println("[RX] ✅ CAN Ready.");

    CanDataPacket_t packet;
    
    for (;;) {
        if (CAN.checkReceive() == CAN_MSGAVAIL) {
            unsigned long canId;
            unsigned char len;
            unsigned char buf[8];

            if (CAN.readMsgBuf(&canId, &len, buf) == CAN_OK) {
                if (canId == CAN_MSG_ID && len >= 5) {
                    /* Extract data (Big-Endian format) */
                    packet.rpmRaw = (uint16_t)((buf[0] << 8) | buf[1]);
                    packet.tempRaw = (uint16_t)((buf[2] << 8) | buf[3]);
                    packet.status = buf[4];
                    packet.timestamp = millis();

                    /* Send to queue with timeout */
                    if (xQueueSend(qCanData, &packet, pdMS_TO_TICKS(10)) != pdPASS) {
                        Serial.println("[RX] ⚠️ Queue Full! Data dropped.");
                    }
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));          /**< Yield CPU to other tasks */
    }
}

/**
 * @brief Task: Data Monitor (Normal Priority)
 * 
 * Receives CAN data from queue, converts to physical values,
 * and detects link loss conditions.
 */
static void taskMonitor(void *pvParameters) {
    (void)pvParameters;
    
    CanDataPacket_t data;
    uint32_t lastValidTimestamp = 0;

    for (;;) {
        /* Blocking receive with timeout for link loss detection */
        if (xQueueReceive(qCanData, &data, pdMS_TO_TICKS(500)) == pdPASS) {
            lastValidTimestamp = data.timestamp;
            
            /* Convert raw values to physical units */
            const float rpm = (float)data.rpmRaw / 10.0f;
            const float temp = (float)data.tempRaw / 10.0f;

            Serial.printf("[MON] 🚗 RPM: %6.1f | TEMP: %5.1f°C | STAT: 0x%02X\n", 
                          rpm, temp, data.status);
        } else {
            /* Timeout - check for link loss */
            if ((millis() - lastValidTimestamp) > TIMEOUT_MS && lastValidTimestamp != 0) {
                Serial.println("[MON] ⚠️ LINK LOST! No data > 3s.");
                lastValidTimestamp = 0;         /**< Reset to prevent spam */
            }
        }
    }
}

/**
 * @brief Initialize system and create FreeRTOS tasks
 */
void setup() {
    Serial.begin(115200);
    delay(1000);                                /**< Stabilize serial port */
    
    Serial.println("\n=================================");
    Serial.println("🏁 FREE_RTOS CAN BUS SLAVE");
    Serial.println("=================================");

    /* Create data queue */
    qCanData = xQueueCreate(QUEUE_SIZE, sizeof(CanDataPacket_t));
    
    if (qCanData == NULL) {
        Serial.println("[ERR] Fatal: Queue creation failed! Halting.");
        while (1);                              /**< Halt on critical failure */
    }

    /* Create CAN RX Task */
    if (xTaskCreate(taskCanRx, "CAN_RX", TASK_STACK_SIZE, NULL, TASK_PRIORITY_RX, NULL) != pdPASS) {
        Serial.println("[ERR] Failed to create RX Task");
    }

    /* Create Monitor Task */
    if (xTaskCreate(taskMonitor, "Monitor", TASK_STACK_SIZE, NULL, TASK_PRIORITY_MON, NULL) != pdPASS) {
        Serial.println("[ERR] Failed to create Monitor Task");
    }
    
    Serial.println("[SYS] ✅ Scheduler Started.");
}

/**
 * @brief Main loop - Idle (all work done in tasks)
 */
void loop() {
    /* Main loop idle - can be used for WDT feeding if needed */
    vTaskDelay(pdMS_TO_TICKS(1000));
}
