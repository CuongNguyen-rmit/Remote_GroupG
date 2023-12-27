#include "Arduino.h"
#include "joystick.h"

int lastLRValue = 0;
int lastUDValue = 0;
const int threshold_joystick = 1; // Threshold for change detection

void joystickInit()
{
    lastLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
    lastUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
}