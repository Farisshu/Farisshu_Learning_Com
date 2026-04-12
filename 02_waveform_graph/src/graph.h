#ifndef GRAPH_H
#define GRAPH_H

#include <Arduino.h>

#define GRAPH_WIDTH 160

void initGraph();
void updateSignal();
uint16_t getSignalY(uint8_t x);
void setFrequency(float hz);
void setAmplitude(float amp);
bool togglePause();
bool isPaused();
void handleGraphCommand(const char* cmd);

#endif