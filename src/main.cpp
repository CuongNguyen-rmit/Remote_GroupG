#include <Controller.h>

void setup() {
  remoteControllerConfig();
}

void loop() {
  // if(button_1.mode == 0) {
  //   Serial.println("zbutton pressed");
  //   button_1.mode = NONE;
  // }
  // else {
  //   Serial.println("ok");
  //   button_1.mode = NONE;
  // }
  buttonPressed(ADC_Read());
  // potentiometerSend(ADC_Read());
  delay(100);
}