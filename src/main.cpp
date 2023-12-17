#include <Controller.h>

void setup() {
  remoteControllerConfig();
  //Serial.begin(115200);
}

void loop() {
  buttonPressed(ADC_Read());
  delay(100);
}