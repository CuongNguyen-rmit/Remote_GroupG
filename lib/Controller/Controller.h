#include <ESP32Servo.h>
#include <esp32_button.h>
#include "../joystick/joystick.h"
#include <esp_now.h>
#include <WiFi.h>
#define BUTTON_RIGHT_PIN 33
#define BUTTON_LEFT_PIN 32
#define BUTTON_STOP_PIN 25
#define STOP 200
#define START 300
#define MIN_SIGNAL 1000
#define MAX_SIGNAL 2000
#define MAX_SIGNAL_STATE 1
#define MIN_SIGNAL_STATE 2
#define YAW_LEFT_STATE 1
#define YAW_RIGHT_STATE 2
#define YAW_NEUTRAL_STATE 0

extern button_t buttonStop;
extern button_t buttonRight;
extern button_t buttonLeft;

typedef struct button_message
{
    int state;
    int id[25];
} button_message;

typedef struct voltage_stuct
{
    int voltageVal;
} voltage_stuct;

// create multiple struct to uniqe the data sent
typedef struct esc_cal_val
{
    int signal;
    int state;
} esc_cal_val;

typedef struct tunning_value_receive
{
    double kp_pitch, kd_pitch, ki_pitch;
    double kp_roll, kd_roll, ki_roll;
    double kp_yaw, kd_yaw, ki_yaw;
    int tunningState;
} tunning_value_receive;

typedef struct imu_struct_receive
{
    float anglex; // Angle in the X direction
    float angley; // Angle in the Y direction
    float anglez; // Angle in the Z direction
    float gyrox;  // Gyroscope reading in the X direction
    float gyroy;  // Gyroscope reading in the Y direction
    float gyroz;  // Gyroscope reading in the Z direction
    int motor1Speed;
    int motor2Speed;
    int motor3Speed;
    int motor4Speed;
    int baseSpeed;
} imu_struct_receive;

typedef struct imu_calibrate_receive
{
    int status;
    char id[98];
} imu_calibrate_receive;

typedef struct joystick_struct_receiver
{
    int joystickx; // j in the X direction
    int joysticky; // j in the Y direction
    char id[10];

} joystick_struct_receiver;

typedef struct joystick_struct_sender
{
    int joystickx; // j in the X direction
    int joysticky; // j in the Y direction
    char id[10];

} joystick_struct_sender;

typedef struct yaw_struct_send
{
    int state;
    char id[45];
} yaw_struct_send;

typedef struct tunning_struct_send
{
    double kpPitch, kdPitch, kiPitch;
    double kpRoll, kdRoll, kiRoll;
    double kpYaw, kdYaw, kiYaw;
    int tunningState;
} tunning_struct_send;

extern imu_struct_receive imuInfoReceiver;
extern joystick_struct_sender joystickSender;
extern button_message myButton;
extern tunning_value_receive pid_info_receive;
extern yaw_struct_send yaw_signal_send;

void esp_now_config();
void buttonInit();
void remoteControllerConfig();
void potentiometerSend(int myVal);
void buttonDataSend(int myVal);
void acctionsHanlder(int val);                        // to controll the action of moving drone
void sendCalSignal(int signalValue, int signalState); // send min signal to tell esc the calibration value (use in last)
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
int ADC_Read();
void sendDataIfJoystickMoved();
void sendJoystickXY(int x, int y);
void tunningCommandSend(int state);
void displayTunningValue();
void sendYawSignal();