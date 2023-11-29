#include <Arduino.h>
#include <esp_now.h>
#include "../lib/potentiometer/potentiometer.h"
#include "../lib/joystick/joystick.h"
#include "../lib/wireless/wireless.h"

// Define the previous values to detect changes
int lastLRValue = 0;
int lastUDValue = 0;
const int threshold = 2; // Threshold for change detection

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
  lastLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
  lastUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
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
  int currentLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
  int currentUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
  if (abs(currentLRValue - lastLRValue) > threshold || abs(currentUDValue - lastUDValue) > threshold)
  {
    lastLRValue = currentLRValue;
    lastUDValue = currentUDValue;
    sendJoystickXY(currentLRValue, currentUDValue);
  }
}

void sendJoystickXY(int x, int y)
{

  struct_msg_Send dataToSend;
  dataToSend.joystickX = x; // Assign values as needed
  dataToSend.joystickY = y;

  // Send data

  esp_now_send(broadcastAddress, (uint8_t *)&dataToSend, sizeof(dataToSend));

  // For debugging
  Serial.print("Joystick moved. LR: ");
  Serial.print(x);
  Serial.print(", UD: ");
  Serial.println(y);
}
