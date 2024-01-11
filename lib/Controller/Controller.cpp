#include <Controller.h>
#include <tunning.h>

// Variables definitions
int potPin = 34;
button_t buttonStop;
button_t buttonRight;
button_t buttonLeft;
button_message myButton;
voltage_stuct myPot;

imu_struct_receive imuInfoReceiver;
joystick_struct_receiver joystickReceiver;
joystick_struct_sender joystickSender;
tunning_struct_send tunningSender;
tunning_value_receive pid_info_receive;
imu_calibrate_receive imuStatusReceive;
yaw_struct_send yaw_signal_send;

// checker declaration
int isStopped;
int yawState;
// ESC struct
esc_cal_val calSignalSender;
uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x20, 0xC1, 0xDC}; // mac address of receiver
void IRAM_ATTR button_isr()
{
  button_update(&buttonStop);
  button_update(&buttonLeft);
  button_update(&buttonRight);
}

void buttonInit()
{
  // Init stop button
  button_add_default(&buttonStop, BUTTON_STOP_PIN);

  // Init right button
  button_add_default(&buttonRight, BUTTON_RIGHT_PIN);

  // Init left button
  button_add_default(&buttonLeft, BUTTON_LEFT_PIN);
  button_init(&button_isr);
  isStopped = 0;
  yawState = YAW_NEUTRAL_STATE;
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
    // if (dataSent == ESP_OK)
    // {
    //   Serial.println("Pot Deliver success");
    // }
    lastSentVal = val; // Update the last sent value
  }
}

void buttonDataSend(int val)
{
  myButton.state = val;
  esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&myButton, sizeof(myButton));
  if (dataSent == ESP_OK)
  {
    Serial.println("Deliver success");
  }
}

void acctionsHanlder(int val)
{

  if (buttonStop.mode == 0)
  {
    potentiometerSend(val);
    // Serial.println(F("ok"));
    sendDataIfJoystickMoved();
    buttonStop.mode = NONE;
  }
  else
  {
    if (isStopped == 0)
    {
      buttonDataSend(STOP);
      isStopped = 1;
    }
    else
    {
      buttonDataSend(START);
      isStopped = 0;
    }
    buttonStop.mode = NONE;
  }
}

void remoteControllerConfig()
{
  buttonInit();
  joystickInit();
  esp_now_config();
  tunningSender.tunningState = 0;
  yaw_signal_send.state = yawState;
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
  case sizeof(pid_info_receive):
    memcpy(&pid_info_receive, incomingData, sizeof(pid_info_receive));
    displayTunningValue();
    break;
  case sizeof(imuStatusReceive):
    memcpy(&imuStatusReceive, incomingData, sizeof(imuStatusReceive));
    Serial.print("Status IMU: ");
    Serial.println(imuStatusReceive.status);
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
    lastUDValue = currentUDValue;
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

void tunningCommandSend(int state)
{
  tunningSender.kpPitch = kp_pitch;
  tunningSender.kiPitch = ki_pitch;
  tunningSender.kdPitch = kd_pitch;

  tunningSender.kpRoll = kp_roll;
  tunningSender.kiRoll = ki_roll;
  tunningSender.kdRoll = kd_roll;

  tunningSender.kpYaw = kp_yaw;
  tunningSender.kiYaw = ki_yaw;
  tunningSender.kdYaw = kd_yaw;
  tunningSender.tunningState = state;
  Serial.println("send data of pid to drone");
  esp_now_send(broadcastAddress, (uint8_t *)&tunningSender, sizeof(tunningSender));
}

void displayTunningValue()
{

  Serial.println("PID Tuning Values");
  Serial.println("================================");

  // Headers
  Serial.println("      | Pitch    | Roll     | Yaw     ");
  Serial.println("--------------------------------");

  // Row for Kp
  Serial.print("Kp    | ");
  Serial.print(pid_info_receive.kp_pitch);
  Serial.print("   | ");
  Serial.print(pid_info_receive.kp_roll);
  Serial.print("   | ");
  Serial.println(pid_info_receive.kp_yaw);

  // Row for Kd
  Serial.print("Kd    | ");
  Serial.print(pid_info_receive.kd_pitch);
  Serial.print("   | ");
  Serial.print(pid_info_receive.kd_roll);
  Serial.print("   | ");
  Serial.println(pid_info_receive.kd_yaw);

  // Row for Ki
  Serial.print("Ki    | ");
  Serial.print(pid_info_receive.ki_pitch);
  Serial.print("   | ");
  Serial.print(pid_info_receive.ki_roll);
  Serial.print("   | ");
  Serial.println(pid_info_receive.ki_yaw);

  Serial.println("================================");
}

void sendYawSignal()
{
  if (buttonLeft.mode == 1)
  {
    yaw_signal_send.state = YAW_LEFT_STATE;
    esp_now_send(broadcastAddress, (uint8_t *)&yaw_signal_send, sizeof(yaw_signal_send));
    yaw_signal_send.state = YAW_NEUTRAL_STATE;
    buttonLeft.mode = NONE;
  }
  else if (buttonRight.mode == 1)
  {
    yaw_signal_send.state = YAW_RIGHT_STATE;
    esp_now_send(broadcastAddress, (uint8_t *)&yaw_signal_send, sizeof(yaw_signal_send));
    yaw_signal_send.state = YAW_NEUTRAL_STATE;
    buttonRight.mode = NONE;
  }
}