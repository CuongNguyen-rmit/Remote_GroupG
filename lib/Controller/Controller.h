
#include <ESP32Servo.h>
#include <esp32_button.h>
#include <esp_now.h>
#include <WiFi.h>
#define BUTTON_1_PIN 33
#define INTERUPT_VAL 200
#define MIN_SIGNAL 1000
#define MAX_SIGNAL 2000


extern button_t button_1;

typedef struct button_message {
    int button_status;
} button_message;

typedef struct voltage_stuct {
    int voltageVal;
} voltage_stuct;

// create multiple struct to uniqe the data sent
typedef struct esc_min_val {
    int minSignal;
    bool state;
} esc_min_val;

typedef struct esc_max_val {
    int maxSignal;
    bool state;
    bool status;
} esc_max_val;

void esp_now_config();
void buttonInit();
void remoteControllerConfig();
void potentiometerSend(int myVal);
void buttonDataSend(int myVal);
void buttonPressed(int val);
void sendMinSignal(int min); // send min signal to tell esc the calibration value (use in last)
void sendMaxSignal(int max); // send max signal to tell esc enter the calibration mode(do it when unplug battery)

int ADC_Read();
