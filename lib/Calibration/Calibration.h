#include <Arduino.h>

void calibrationModeWithoutBattery(); // step 1 calibrate without battery first
void calibrationModeWithBattery(); // step2 calibrate when connect to battery after enter calibration mode
void calibrationModeWithValue(); // to tell user about the esc value
void waitForKeyStroke();