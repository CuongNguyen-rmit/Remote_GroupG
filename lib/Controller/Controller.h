
#include <ESP32Servo.h>
#include <esp32_button.h>
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

typedef struct sent_pid_tunning {
    char charSent;
    char suffCharSent;
    int id[50];
} sent_pid_tunning;

void esp_now_config();
void buttonInit();
void remoteControllerConfig();
void potentiometerSend(int myVal);
void buttonDataSend(int myVal);
void buttonPressed(int val);
void sendCalSignal(int signalValue, int signalState); // send min signal to tell esc the calibration value (use in last)
void sendTunningCommand();

int ADC_Read();
