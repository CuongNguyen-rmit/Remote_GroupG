#include <Arduino.h>
#include "potentiometer.h"

int CtrlPWM;                 // Control Signal. Varies between [0 - 180]
unsigned long time_prev = 0; // Variable used for serial monitoring

void SerialDataPrintPotentiometer()
{
    if (micros() - time_prev >= 20000)
    {
        time_prev = micros();
        Serial.print(millis());
        Serial.print("\t");
        Serial.println(CtrlPWM);
    }
}

void mapPotentiometerValue()
{
    CtrlPWM = map(analogRead(POT_PIN), 0, 4095, 0, 180);
}
