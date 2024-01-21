#include <Controller.h>
#include <Calibration.h>
#include <tunning.h>

void printIMUData(const imu_struct_receive &imuData);
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
  Serial.print(imuInfoReceiver.anglex);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.angley);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.anglez);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.motor1Speed);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.motor2Speed);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.motor3Speed);
  Serial.print(" ");
  Serial.print(imuInfoReceiver.motor4Speed);
  Serial.println(" ");


  acctionsHanlder(ADC_Read());
  sendYawSignal();
  delay(50);
  // if (welcomeMenu())
  // {
  //   menuPrompt();
  //   tunningCommandSend(tunning_state);
  // }
}

void printIMUData(const imu_struct_receive &imuData)
{
  Serial.print("Angles: X=");
  Serial.print(imuData.anglex);
  Serial.print(", Y=");
  Serial.print(imuData.angley);
  Serial.print(", Z=");
  Serial.print(imuData.anglez);
  Serial.print(" | Gyro: X=");
  Serial.print(imuData.gyrox);
  Serial.print(", Y=");
  Serial.print(imuData.gyroy);
  Serial.print(", Z=");
  Serial.print(imuData.gyroz);
  Serial.print(" | Motor Speeds: 1=");
  Serial.print(imuData.motor1Speed);
  Serial.print(", 2=");
  Serial.print(imuData.motor2Speed);
  Serial.print(", 3=");
  Serial.print(imuData.motor3Speed);
  Serial.print(", 4=");
  Serial.print(imuData.motor4Speed);
  Serial.print(", Base=");
  Serial.println(imuData.baseSpeed);
}
