#include "./Calibration.h"

void waitForKeyStroke()
{
  while (!Serial.available())
    ;
  while (Serial.available())
    Serial.read();
}

void calibrationModeWithoutBattery()
{
  Serial.println();
  Serial.println("Calibration step 1. Disconnect the battery.");
  Serial.println("Press any key to continue.");
  waitForKeyStroke();
}

void calibrationModeWithBattery()
{
  Serial.println();
  Serial.println("Calibration step 2. Connect the battery.");
  Serial.println("Wait for two short bips.");
  Serial.println("Press any key to continue.");
  waitForKeyStroke();
}

void calibrationModeEnd()
{
  Serial.println();
  Serial.println("Wait for 4 short bips, and one long bip.");
  Serial.println("Press any key to finish.");
  waitForKeyStroke();
  Serial.println("Calibration Completed");
}