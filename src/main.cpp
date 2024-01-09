#include <Controller.h>
#include <Calibration.h>
#include <tunning.h>
void setup()
{
  Serial.begin(115200);
  remoteControllerConfig();
  // initialize the esc
  // step1: disconnect battery
  calibrationModeWithoutBattery();             // tell user calibrate when disconnect battery
  sendCalSignal(MAX_SIGNAL, MAX_SIGNAL_STATE); // send max signal to tell esc enter the calibration mode

  // // Step 2: connect battery
  calibrationModeWithBattery();                // tell user calibrate with battery
  sendCalSignal(MIN_SIGNAL, MIN_SIGNAL_STATE); // send min signal to tell esc the calibrate value
  calibrationModeEnd();                        // tell user wait for the beep sound then controll with pot
}

void loop()
{
  int userChoice = welcomeMenu();
  while(Serial.available() == 0) {
    acctionsHanlder(ADC_Read());
    Serial.println("BaseSpeed | Motor1 | Motor2 | Motor3 | Motor4");
    Serial.print(imuInfoReceiver.baseSpeed);
    Serial.print("         | ");
    Serial.print(imuInfoReceiver.motor1Speed);
    Serial.print("         | ");
    Serial.print(imuInfoReceiver.motor2Speed);
    Serial.print("         | ");
    Serial.print(imuInfoReceiver.motor3Speed);
    Serial.print("         | ");
    Serial.println(imuInfoReceiver.motor4Speed);
    Serial.println(kp_pitch);
    delay(500);
  }
  if(welcomeMenu()) {
    menuPrompt();
    displayTunningValue();
    tunningCommandSend(tunning_state);
  }
  Serial.println(ADC_Read());

  // Serial.print("Received IMU data:");
  // Serial.print(" Angle X: ");
  // Serial.print(imuInfoReceiver.anglex);
  // Serial.print(" Angle Y: ");
  // Serial.print(imuInfoReceiver.angley);
  // Serial.print(" Angle Z: ");
  // Serial.print(imuInfoReceiver.anglez);
  // Serial.print(" Gyro X: ");
  // Serial.print(imuInfoReceiver.gyrox);
  // Serial.print(" Gyro Y: ");
  // Serial.print(imuInfoReceiver.gyroy);
  // Serial.print(" Gyro Z: ");
  // Serial.print(imuInfoReceiver.gyroz);
  // Serial.print(" Speed1: ");
  // Serial.print(imuInfoReceiver.motor1Speed);
  // Serial.print(" Speed2: ");
  // Serial.print(imuInfoReceiver.motor2Speed);
  // Serial.print(" Speed3: ");
  // Serial.print(imuInfoReceiver.motor3Speed);
  // Serial.print(" Speed4: ");
  // Serial.print(imuInfoReceiver.motor4Speed);
  // Serial.print(" BaseSpeed: ");
  // Serial.println(imuInfoReceiver.baseSpeed);

  delay(100);
}