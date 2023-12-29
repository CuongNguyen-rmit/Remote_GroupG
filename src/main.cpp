#include <Controller.h>
#include <Calibration.h>
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
  acctionsHanlder(ADC_Read());
  menuPrompt();
  tunningCommandSend();
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