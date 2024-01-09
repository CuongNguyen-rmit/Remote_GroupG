#include <ESP32Servo.h>
#include <esp32_button.h>
#include "../joystick/joystick.h"
#include <esp_now.h>
#include <WiFi.h>
#define BUTTON_1_PIN 33
#define INTERUPT_VAL 200
#define MIN_SIGNAL 1000
#define MAX_SIGNAL 2000
#define MAX_SIGNAL_STATE 1
#define MIN_SIGNAL_STATE 2

extern button_t button_1;

typedef struct button_message
{
    int button_status;
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

typedef struct joystick_struct_receiver
{
    float joystickx; // j in the X direction
    float joysticky; // j in the Y direction
    char id[10];

} joystick_struct_receiver;

typedef struct joystick_struct_sender
{
    float joystickx; // j in the X direction
    float joysticky; // j in the Y direction
    char id[10];

} joystick_struct_sender;

typedef struct tunning_struct_send {
    float kpPitch,kdPitch,kiPitch;
    float kpRoll,kdRoll,kiRoll;
    float kpYaw,kdYaw,kiYaw;
} tunning_struct_send;

extern imu_struct_receive imuInfoReceiver;
extern joystick_struct_sender joystickSender;


void esp_now_config();
void buttonInit();
void remoteControllerConfig();
void potentiometerSend(int myVal);
void buttonDataSend(int myVal);
void acctionsHanlder(int val); // to controll the action of moving drone
void sendCalSignal(int signalValue, int signalState); // send min signal to tell esc the calibration value (use in last)
void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len);
int ADC_Read();
void sendDataIfJoystickMoved();
void sendJoystickXY(int x, int y);
void tunningCommandSend();
