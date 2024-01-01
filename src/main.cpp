#include <Controller.h>
#include <Calibration.h>
#include <Arduino.h>

void setup()
{
  Serial.begin(115200);
  // remoteControllerConfig();
  // // initialize the esc
  // // step1: disconnect battery
  // calibrationModeWithoutBattery();             // tell user calibrate when disconnect battery
  // sendCalSignal(MAX_SIGNAL, MAX_SIGNAL_STATE); // send max signal to tell esc enter the calibration mode

  // // // Step 2: connect battery
  // calibrationModeWithBattery();                // tell user calibrate with battery
  // sendCalSignal(MIN_SIGNAL, MIN_SIGNAL_STATE); // send min signal to tell esc the calibrate value
  // calibrationModeEnd();                        // tell user wait for the beep sound then controll with pot
}

void loop()
{
    // int userChoice = welcomeMenu();
  if(welcomeMenu()) {
    menuPrompt();
    displayTunningValue();
  }
  else {
    //acctionsHanlder(ADC_Read());
  }
  tunningCommandSend();
  delay(100);
}