// ✅ Unity Test Framework + CppUTest
#include <unity.h>
#include <stdint.h>
#include <string.h>

// --- Struktur Data (Sama persis dengan main.cpp) ---
typedef struct {
    uint16_t rpm_raw;
    uint16_t temp_raw;
    uint8_t  status;
    uint32_t timestamp;
} CanDataPacket_t;

// --- Fungsi yang Di-Test: Parser Logic ---
// Kita pisahkan logika parsing agar bisa di-test tanpa hardware
bool parse_can_payload(const uint8_t* buf, uint8_t len, CanDataPacket_t* out) {
    if (buf == NULL || out == NULL || len < 5) return false;
    
    // Big-Endian decoding (Network Byte Order)
    out->rpm_raw  = (uint16_t)((buf[0] << 8) | buf[1]);
    out->temp_raw = (uint16_t)((buf[2] << 8) | buf[3]);
    out->status   = buf[4];
    out->timestamp = 0; // Tidak ada timestamp di payload
    
    return true;
}

// --- TEST CASE 1: Parsing Normal ---
void test_parse_valid_frame(void) {
    // Input: RPM=800.0 (8000), Temp=90.5 (905), Status=OK
    uint8_t mock_buf[8] = {0x1F, 0x40, 0x03, 0x89, 0x00, 0x00, 0x00, 0x00};
    CanDataPacket_t result;
    
    TEST_ASSERT_TRUE(parse_can_payload(mock_buf, 8, &result));
    TEST_ASSERT_EQUAL_UINT16(8000, result.rpm_raw);
    TEST_ASSERT_EQUAL_UINT16(905, result.temp_raw);
    TEST_ASSERT_EQUAL_UINT8(0x00, result.status);
}

// --- TEST CASE 2: Parsing Data Batas (Edge Case) ---
void test_parse_edge_cases(void) {
    uint8_t buf[8];
    CanDataPacket_t result;

    // Min RPM (700.0 = 7000 = 0x1B58)
    memset(buf, 0, 8);
    buf[0] = 0x1B; buf[1] = 0x58; buf[2] = 0x02; buf[3] = 0xEE; buf[4] = 0x00; // 7000, 750
    TEST_ASSERT_TRUE(parse_can_payload(buf, 5, &result));
    TEST_ASSERT_EQUAL_UINT16(7000, result.rpm_raw);

    // Max Status (CRIT = 0x02)
    buf[4] = 0x02;
    parse_can_payload(buf, 5, &result);
    TEST_ASSERT_EQUAL_UINT8(0x02, result.status);
}

// --- TEST CASE 3: Invalid Input (Safety Check) ---
void test_parse_invalid_input(void) {
    CanDataPacket_t result;
    uint8_t short_buf[3] = {0x01, 0x02, 0x03}; // Too short (< 5 bytes)
    
    // Harus return false karena data tidak lengkap
    TEST_ASSERT_FALSE(parse_can_payload(short_buf, 3, &result));
    TEST_ASSERT_FALSE(parse_can_payload(NULL, 8, &result)); // Null buffer
}

// --- Unity Setup & Teardown ---
void setUp(void) {}
void tearDown(void) {}

// --- Main Entry Point ---
int main(int argc, char** argv) {
    UNITY_BEGIN();
    
    RUN_TEST(test_parse_valid_frame);
    RUN_TEST(test_parse_edge_cases);
    RUN_TEST(test_parse_invalid_input);
    
    return UNITY_END();
}