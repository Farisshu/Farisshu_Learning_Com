#ifndef SIMULATOR_H
#define SIMULATOR_H

// Inisialisasi simulator
void initSimulator();

// Update RPM simulation
void updateSimulator();

// Get current RPM
float getCurrentRPM();

// Set target RPM
void setTargetRPM(float rpm);

// Toggle auto mode
bool toggleAutoMode();

// Get auto mode status
bool isAutoMode();

// Handle serial command
void handleCommand(const char* cmd);

#endif