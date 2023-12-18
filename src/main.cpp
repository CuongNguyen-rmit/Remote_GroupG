#include <Arduino.h>
#include <esp_now.h>
#include "../lib/potentiometer/potentiometer.h"
#include "../lib/joystick/joystick.h"
#include "../lib/wireless/wireless.h"
#include "../lib/button/button.h"
#include <WiFi.h>

// Define the previous values to detect changes
int lastLRValue = 0;
int lastUDValue = 0;
int lastPOTValue = 0;
const int threshold_joystick = 1; // Threshold for change detection
const int threshold_pot = 10;
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 0;

// Function Declarations
void Init_Serial();
void IRAM_ATTR handleButtonLeftPress();
void IRAM_ATTR handleButtonRightPress();
void IRAM_ATTR handleButtonEmPress();
void sendDataIfJoystickMoved();
void sendJoystickXY(int x, int y);
void sendDataIfPOTChange();

void setup()
{
  Init_Serial();
  espnow_initialize();
  pinMode(BUTTON_LEFT, INPUT_PULLDOWN);
  pinMode(BUTTON_RIGHT, INPUT_PULLDOWN);
  pinMode(BUTTON_EMERGENCY, INPUT_PULLDOWN);
  // attachInterrupt(digitalPinToInterrupt(SW_PIN), handleJoystickButtonPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_LEFT), handleButtonLeftPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), handleButtonRightPress, FALLING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_EMERGENCY), handleButtonEmPress, FALLING);

  Serial.println("Welcome to Drone's Remote Controller");
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
  // Read initial joystick values
  lastLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
  lastUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
  lastPOTValue = map(analogRead(POT_PIN), 0, 4095, 0, 180);
}

void loop()
{
  sendDataIfJoystickMoved();
  sendDataIfPOTChange();
}

// Function Definitions
void Init_Serial()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for Serial to initialize
}

void IRAM_ATTR handleButtonLeftPress()
{
  // debounce button
  unsigned long currentMillis = millis();
  if ((currentMillis - lastDebounceTime) > debounceDelay)
  {
    Serial.println("button left");
    // Update the last debounce time
    lastDebounceTime = currentMillis;
    struct_msg_Send_Button dataToSend = {2, true, "button pressed"};
    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

void IRAM_ATTR handleButtonRightPress()
{
  // debounce button
  unsigned long currentMillis = millis();
  if ((currentMillis - lastDebounceTime) > debounceDelay)
  {
    Serial.println("button right");
    // Update the last debounce time
    lastDebounceTime = currentMillis;
    struct_msg_Send_Button dataToSend = {1, true, "button pressed"};
    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

void IRAM_ATTR handleButtonEmPress()
{
  // debounce button
  unsigned long currentMillis = millis();
  if ((currentMillis - lastDebounceTime) > debounceDelay)
  {
    Serial.println("button em");
    // Update the last debounce time
    lastDebounceTime = currentMillis;
    struct_msg_Send_Button dataToSend = {3, true, "button pressed"};

    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

void sendDataIfJoystickMoved()
{
  int currentLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
  int currentUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
  if (abs(currentLRValue - lastLRValue) > threshold_joystick || abs(currentUDValue - lastUDValue) > threshold_joystick)
  {
    Serial.print("Joystick X: ");
    Serial.print(currentLRValue);
    Serial.print(", Joystick Y: ");
    Serial.println(currentLRValue);
    lastLRValue = currentLRValue;
    lastUDValue = currentLRValue;
    sendJoystickXY(currentLRValue, currentUDValue);
  }
}

void sendDataIfPOTChange()
{
  int currentPOTValue = map(analogRead(POT_PIN), 0, 4095, 0, 180);
  if (abs(currentPOTValue - lastPOTValue) > threshold_pot)
  {
    lastPOTValue = currentPOTValue;
    Serial.print("POT : ");
    Serial.println(currentPOTValue);
    struct_msg_Send_POT dataToSend;
    dataToSend.value = currentPOTValue;
    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}

void sendJoystickXY(int x, int y)
{

  struct_msg_Send dataToSend;
  dataToSend.joystickX = x;
  dataToSend.joystickY = y;

  // Send data

  esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
}
