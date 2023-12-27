#include "Arduino.h"
#include "joystick.h"

int lastLRValue;
int lastUDValue;
int threshold_joystick; // Threshold for change detection

void joystickInit()
{
    Serial.println("joystick init");
    threshold_joystick = 1;
    lastLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
    lastUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
}