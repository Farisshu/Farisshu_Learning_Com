#ifndef RS485_PARSER_H
#define RS485_PARSER_H

#include <Arduino.h>

typedef struct {
  float temp;
  float pressure;
  bool valid;
} Rs485Data_t;

void rs485_init(uint32_t baud, int8_t rxPin, int8_t txPin);
void rs485_poll(Rs485Data_t* outData);
bool rs485_isLinkAlive(uint32_t timeoutMs);

#endif