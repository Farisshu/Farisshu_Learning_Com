#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>

// Inisialisasi display
void initDisplay();

// Draw motor RPM display
void drawMotorDisplay(float rpm);

// Clear screen dengan gradient
void clearScreenWithGradient();

#endif