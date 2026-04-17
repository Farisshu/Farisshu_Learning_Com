#include <Arduino.h>
#include <SPI.h>
#include <mcp_can.h>

// FreeRTOS Headers
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// --- Configuration Constants (Magic Numbers Removal) ---
#define CAN_CS_PIN          22
#define CAN_BAUDRATE        CAN_250KBPS
#define CAN_OSC_FREQ        MCP_8MHZ
#define CAN_MSG_ID          0x100UL
#define QUEUE_SIZE          5
#define TIMEOUT_MS          3000UL

// --- Hardware Instance ---
MCP_CAN CAN(CAN_CS_PIN);

// --- Data Structure ---
typedef struct {
    uint16_t rpm_raw;
    uint16_t temp_raw;
    uint8_t  status;
    uint32_t timestamp; // Gunakan timestamp daripada flag boolean untuk presisi
} CanDataPacket_t;

// --- Global Handles (Static untuk membatasi scope) ---
static QueueHandle_t qCanData = NULL;

// ---------------------------------------------------------
// TASK 1: CAN RECEIVER (Priority High)
// ---------------------------------------------------------
static void task_can_rx(void *pvParameters) {
    (void)pvParameters; // ✅ Supress "unused parameter" warning

    Serial.println("[RX] Initializing CAN Controller...");
    
    // Loop sampai hardware siap
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
                    // ✅ Explicit Casting untuk keamanan tipe data
                    packet.rpm_raw  = (uint16_t)((buf[0] << 8) | buf[1]);
                    packet.temp_raw = (uint16_t)((buf[2] << 8) | buf[3]);
                    packet.status   = buf[4];
                    packet.timestamp = millis();

                    // ✅ Kirim ke Queue. Jika penuh, tunggu maksimal 10 tick
                    if (xQueueSend(qCanData, &packet, pdMS_TO_TICKS(10)) != pdPASS) {
                        Serial.println("[RX] ⚠️ Queue Full! Data dropped.");
                    }
                }
            }
        }
        // Yield CPU ke task lain
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

// ---------------------------------------------------------
// TASK 2: MONITOR & LOGIC (Priority Normal)
// ---------------------------------------------------------
static void task_monitor(void *pvParameters) {
    (void)pvParameters;
    
    CanDataPacket_t data;
    uint32_t last_valid_timestamp = 0;

    for (;;) {
        // ✅ Blocking Receive: Task akan "tidur" efisien sampai data ada
        // Timeout 500ms untuk cek Link Loss
        if (xQueueReceive(qCanData, &data, pdMS_TO_TICKS(500)) == pdPASS) {
            last_valid_timestamp = data.timestamp;
            
            // Floating point calculation (disengaja di sini agar Task RX ringan)
            float rpm  = (float)data.rpm_raw / 10.0f;
            float temp = (float)data.temp_raw / 10.0f;

            Serial.printf("[MON] 🚗 RPM: %6.1f | TEMP: %5.1f C | STAT: 0x%02X\n", 
                          rpm, temp, data.status);
        } else {
            // ✅ Timeout Detection Logic
            if ((millis() - last_valid_timestamp) > TIMEOUT_MS && last_valid_timestamp != 0) {
                Serial.println("[MON] ⚠️ LINK LOST! No data > 3s.");
                last_valid_timestamp = 0; // Reset agar log tidak spam
            }
        }
    }
}

// ---------------------------------------------------------
// SETUP
// ---------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(1000); // Stabilisasi Serial
    Serial.println("\n=================================");
    Serial.println("🏁 FREE_RTOS CAN BUS (Static Analysis Clean)");
    Serial.println("=================================");

    // ✅ Buat Queue
    qCanData = xQueueCreate(QUEUE_SIZE, sizeof(CanDataPacket_t));
    
    if (qCanData == NULL) {
        Serial.println("[ERR] Fatal: Queue creation failed! Halting.");
        while(1); // Stop eksekusi jika resource gagal
    }

    // ✅ Create Tasks
    // Priority: RX=2 (Penting), Monitor=1 (Standar)
    if (xTaskCreate(task_can_rx, "CAN_RX", 4096, NULL, 2, NULL) != pdPASS) {
        Serial.println("[ERR] Failed to create RX Task");
    }

    if (xTaskCreate(task_monitor, "Monitor", 4096, NULL, 1, NULL) != pdPASS) {
        Serial.println("[ERR] Failed to create Monitor Task");
    }
    
    Serial.println("[SYS] ✅ Scheduler Started.");
}

void loop() {
    // Loop utama kosong.
    // Kita bisa pakai ini untuk Update Watchdog Timer (WDT) jika diaktifkan
    vTaskDelay(pdMS_TO_TICKS(1000));
}