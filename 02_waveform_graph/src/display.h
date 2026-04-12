#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

void initDisplay();
void drawGraph(uint16_t (*getY)(uint8_t));
void drawGrid();

#endif