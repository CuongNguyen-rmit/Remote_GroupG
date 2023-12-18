#include <Controller.h>
#include <Calibration.h>

void setup() {
  remoteControllerConfig();
  // initialize the esc
  // step1: disconnect battery
  calibrationModeWithoutBattery(); // tell user calibrate when disconnect battery
  sendMaxSignal(MAX_SIGNAL); // send max signal to tell esc enter the calibration mode

  // Step 2: connect battery
  calibrationModeWithBattery(); // tell user calibrate with battery
  sendMinSignal(MIN_SIGNAL); // send min signal to tell esc the calibrate value
  calibrationModeEnd(); // tell user wait for the beep sound then controll with pot
  //Serial.begin(115200);
}

void loop() {
  buttonPressed(ADC_Read());
  delay(100);
}