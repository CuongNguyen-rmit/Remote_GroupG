#include <Arduino.h>
#include <Calibration.h>
#define PITCH_FLAG 1
#define ROLL_FLAG 2
#define YAW_FLAG 3
// declare variables
extern double kp_pitch, kd_pitch, ki_pitch;
extern double kp_roll, kd_roll, ki_roll;
extern double kp_yaw, kd_yaw, ki_yaw;
extern int tunning_state; //to check if user want to use current tunning
void menuPrompt();
// the flag is used to check which axis is selected
// This function create menu for user to tunning
void readTunningValue(int flag);
double extractNumber(const char *input);
void tunningMenu(int flag); // the flag is used to check which axis is selected
int welcomeMenu();
