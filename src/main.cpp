#include <Controller.h>
#include <Calibration.h>

void setup()
{
  remoteControllerConfig();
  // initialize the esc
  // step1: disconnect battery
  calibrationModeWithoutBattery();             // tell user calibrate when disconnect battery
  sendCalSignal(MAX_SIGNAL, MAX_SIGNAL_STATE); // send max signal to tell esc enter the calibration mode

  // Step 2: connect battery
  calibrationModeWithBattery();                // tell user calibrate with battery
  sendCalSignal(MIN_SIGNAL, MIN_SIGNAL_STATE); // send min signal to tell esc the calibrate value
  calibrationModeEnd();                        // tell user wait for the beep sound then controll with pot
  // Serial.begin(115200);
}

void loop()
{
  buttonPressed(ADC_Read());
  delay(100);
}