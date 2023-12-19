#include <Arduino.h>
/*Disclaimer: Users must follow the step displayed in serial monitor to calibrate*/

void calibrationModeWithoutBattery(); // step 1 calibrate without battery first
void calibrationModeWithBattery(); // step2 calibrate when connect to battery after enter calibration mode
void calibrationModeEnd(); // to tell user to wait for the beep sound
void waitForKeyStroke(); // this  function to receive the input from keyboard