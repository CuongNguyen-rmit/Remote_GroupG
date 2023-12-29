#include <Arduino.h>
#include "../Calibration/Calibration.h"
#define PITCH_FLAG 1
#define ROLL_FLAG 2
#define YAW_FLAG 3
// declare variables
extern double kp_pitch, kd_pitch, ki_pitch;
extern double kp_roll, kd_roll, ki_roll;
extern double kp_yaw, kd_yaw, ki_yaw;
void menuPrompt();
// the flag is used to check which axis is selected 
// This function create menu for user to tunning
void pitchTunning(int flag);
double extractNumber(const char * input);
void setTunning(int flag); // the flag is used to check which axis is selected