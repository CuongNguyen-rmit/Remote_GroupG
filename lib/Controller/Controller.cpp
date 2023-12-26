#include "./Controller.h"

// Variables definitions
int potPin = 34;
button_t button_1;
button_message myButton;
voltage_stuct myPot;

imu_struct_receive imuInfoReceiver;
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
  Serial.begin(115200);
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
    // if (dataSent == ESP_OK) {
    //     Serial.println("Deliver success");
    // }

    Serial.print("pot: ");
    Serial.println(myPot.voltageVal);

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

void buttonPressed(int val)
{
  if (button_1.mode == 0)
  {
    potentiometerSend(val);
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
  esp_now_config();
}

void sendCalSignal(int signalValue, int signalState)
{
  calSignalSender.signal = signalValue;
  calSignalSender.state = signalState;
  esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&calSignalSender, sizeof(calSignalSender));
  if (dataSent == ESP_OK)
  {
    Serial.println("Deliver success");
    Serial.print("signal state: ");
    Serial.println(calSignalSender.state);
    Serial.print("signal value: ");
    Serial.println(calSignalSender.signal);
  }
}

void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  // Serial.println("here");
  switch (len)
  {
  case sizeof(imuInfoReceiver):

    memcpy(&imuInfoReceiver, incomingData, sizeof(imuInfoReceiver));
    // serial print here
    // Serial print received data
    Serial.println("Received IMU data:");
    Serial.print(" Angle X: ");
    Serial.print(imuInfoReceiver.anglex);
    Serial.print(" Angle Y: ");
    Serial.print(imuInfoReceiver.angley);
    Serial.print(" Angle Z: ");
    Serial.print(imuInfoReceiver.anglez);
    Serial.print(" Gyro X: ");
    Serial.print(imuInfoReceiver.gyrox);
    Serial.print(" Gyro Y: ");
    Serial.print(imuInfoReceiver.gyroy);
    Serial.print(" Gyro Z: ");
    Serial.println(imuInfoReceiver.gyroz);

    break;

  default:
    // Handle unexpected data length
    Serial.println("Received data of unexpected length.");
    break;
  }
}
