#include <Arduino.h>
#include <esp_now.h>
#include "../lib/potentiometer/potentiometer.h"
#include "../lib/joystick/joystick.h"
#include "../lib/wireless/wireless.h"

// Define the previous values to detect changes
int lastLRValue = 0;
int lastUDValue = 0;
const int threshold = 100; // Threshold for change detection

// Function Declarations
void Init_Serial();
void IRAM_ATTR handleJoystickButtonPress();
void sendDataIfJoystickMoved();
void sendJoystickXY(int x, int y);

void setup()
{
  Init_Serial();
  espnow_initialize();
  // pinMode(LR_PIN, INPUT);
  // pinMode(UD_PIN, INPUT);
  pinMode(SW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(SW_PIN), handleJoystickButtonPress, FALLING);

  Serial.println("Welcome to Drone's Remote Controller");

  // Read initial joystick values
  lastLRValue = analogRead(LR_PIN);
  lastUDValue = analogRead(UD_PIN);
}

void loop()
{
  sendDataIfJoystickMoved();
}

// Function Definitions
void Init_Serial()
{
  Serial.begin(9600);
  while (!Serial)
    ; // Wait for Serial to initialize
}

void IRAM_ATTR handleJoystickButtonPress()
{
  Serial.println("Joystick button pressed");
  // Add any specific code to handle button press
}

void sendDataIfJoystickMoved()
{
  int currentLRValue = analogRead(LR_PIN);
  int currentUDValue = analogRead(UD_PIN);
  delay(1000);
  if (abs(currentLRValue - lastLRValue) > threshold || abs(currentUDValue - lastUDValue) > threshold)
  {
    struct_msg_Send dataToSend;
    dataToSend.joystickX = currentLRValue; // Assign values as needed
    dataToSend.joystickY = lastLRValue;

    // Send data

    esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));
  }
}
