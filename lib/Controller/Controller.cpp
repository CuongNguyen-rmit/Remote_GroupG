#include "./Controller.h"

// Variables definitions
int potPin = 34;
button_t button_1;
button_message myButton;
voltage_stuct myPot;

imu_struct_receive imuInfoReceiver;
joystick_struct_receiver joystickReceiver;
joystick_struct_sender joystickSender;
tunning_struct_send tunningSender;
// ESC struct
esc_cal_val calSignalSender;
uint8_t broadcastAddress[] = {0x48, 0xE7, 0x29, 0x93, 0xD8, 0x24}; // mac address of receiver
void IRAM_ATTR button_isr()
{
  button_update(&button_1);
}

void buttonInit()
{
  button_add_default(&button_1, BUTTON_1_PIN);
  button_init(&button_isr);
}

void onDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{ // this is called when data is transmitted
}

void esp_now_config()
{
  WiFi.mode(WIFI_STA); //--> Set device as a Wi-Fi Station.
  //----------------------------------------Init ESP-NOW
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  //----------------------------------------Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(onDataSent); // call back function to getthe sent status into the  ondatasent
  esp_now_register_recv_cb(onDataReceived);
  //----------------------------------------Register peer
  esp_now_peer_info_t peerInfo = {};
  // memset(&peerInfo, 0, sizeof(peerInfo)); // set peer, it could affect wifi
  memcpy(peerInfo.peer_addr, broadcastAddress, sizeof(broadcastAddress));
  peerInfo.channel = 0;
  peerInfo.encrypt = false;
  //----------------------------------------

  //----------------------------------------Add peer
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }
  else
  {
    Serial.println("succssefull to add peer");
    return;
  }
}

int ADC_Read()
{
  return map(analogRead(potPin), 0, 4095, 0, 180);
}

void potentiometerSend(int val)
{
  static int lastSentVal = -1; // Initialize with -1 to ensure the first value is always sent

  // Check if the difference between the current value and the last sent value is 5 or more
  if (abs(val - lastSentVal) >= 5)
  {
    myPot.voltageVal = val;
    esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&myPot, sizeof(myPot));
    if (dataSent == ESP_OK)
    {
      Serial.println("Pot Deliver success");
    }
    lastSentVal = val; // Update the last sent value
  }
}

void buttonDataSend(int val)
{
  myButton.button_status = val;
  esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&myButton, sizeof(myPot));
  if (dataSent == ESP_OK)
  {
    Serial.println("Deliver success");
  }
  Serial.println(myButton.button_status);
}

void acctionsHanlder(int val)
{
  Serial.print("button mode ");
  Serial.println(button_1.mode);
  if (button_1.mode == 0)
  {
    potentiometerSend(val);
    //Serial.println("ok");
    sendDataIfJoystickMoved();
    button_1.mode = NONE;
  }
  else
  {
    buttonDataSend(INTERUPT_VAL);

    button_1.mode = NONE;
  }
}

void remoteControllerConfig()
{
  buttonInit();
  joystickInit();
  esp_now_config();
}


void sendCalSignal(int signalValue, int signalState)
{
  calSignalSender.signal = signalValue;
  calSignalSender.state = signalState;
  esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&calSignalSender, sizeof(calSignalSender));
}

void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Serial.println("here");
  switch (len)
  {
  case sizeof(imuInfoReceiver):

    memcpy(&imuInfoReceiver, incomingData, sizeof(imuInfoReceiver));
    break;

  case sizeof(joystickReceiver):
    memcpy(&joystickReceiver, incomingData, sizeof(joystickReceiver));
    Serial.print("Joyx: ");
    Serial.print(joystickReceiver.joystickx);
    Serial.print(" Joyy: ");
    Serial.println(joystickReceiver.joysticky);

    break;

  default:
    // Handle unexpected data length
    // Serial.println("Received data of unexpected length.");
    break;
  }
}

void sendDataIfJoystickMoved()
{

  int currentLRValue = map(analogRead(LR_PIN), 0, 4095, 0, 12);
  int currentUDValue = map(analogRead(UD_PIN), 0, 4095, 0, 12);
  if (abs(currentLRValue - lastLRValue) > threshold_joystick || abs(currentUDValue - lastUDValue) > threshold_joystick)
  {
    lastLRValue = currentLRValue;
    lastUDValue = currentLRValue;
    sendJoystickXY(currentLRValue, currentUDValue);
  }
}

void sendJoystickXY(int x, int y)
{

  joystickSender.joystickx = x;
  joystickSender.joysticky = y;

  // Send data

  esp_now_send(broadcastAddress, (uint8_t *)&joystickSender, sizeof(joystickSender));
}

void tunningCommandSend() {
  tunningSender.kpPitch = kp_pitch;
  tunningSender.kiPitch = ki_pitch;
  tunningSender.kdPitch = kd_pitch;

  tunningSender.kpRoll = kp_roll;
  tunningSender.kiRoll = ki_roll;
  tunningSender.kdRoll = kd_roll;

  tunningSender.kpYaw = kp_yaw;
  tunningSender.kiYaw = ki_yaw;
  tunningSender.kdYaw = kd_yaw;
  esp_now_send(broadcastAddress, (uint8_t *)&tunningSender, sizeof(tunningSender));
}
