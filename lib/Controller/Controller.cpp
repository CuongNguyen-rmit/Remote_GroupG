#include <Controller.h>
#include <tunning.h>

// Variables definitions
int potPin = 34;
button_t button_1;
button_message myButton;
voltage_stuct myPot;

imu_struct_receive imuInfoReceiver;
joystick_struct_receiver joystickReceiver;
joystick_struct_sender joystickSender;
tunning_struct_send tunningSender;
tunning_value_receive pid_info_receive;
imu_calibrate_receive imuStatusReceive;

int isStopped;
// ESC struct
esc_cal_val calSignalSender;
uint8_t broadcastAddress[] = {0xE4, 0x65, 0xB8, 0x20, 0xC1, 0xDC}; // mac address of receiver
void IRAM_ATTR button_isr()
{
  button_update(&button_1);
}

void buttonInit()
{
  button_add_default(&button_1, BUTTON_3_PIN);
  button_init(&button_isr);
  isStopped = 0;
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

  // Simplify the structure by eliminating nested if-else where possible
    if (button_1.mode == 0) {
        potentiometerSend(val);
        sendDataIfJoystickMoved();
    } else {
        // Use a ternary operator for concise code
        buttonDataSend(isStopped == 0 ? STOP : START);
        // Toggle isStopped
        isStopped = (isStopped == 1) ? 0 : 1;
    }
    
    button_1.mode = NONE;
}

void remoteControllerConfig()
{
  buttonInit();
  joystickInit();
  esp_now_config();
  tunningSender.tunningState = 0;
}

void sendCalSignal(int signalValue, int signalState) // send calibration signal
{
  calSignalSender.signal = signalValue;
  calSignalSender.state = signalState;
  esp_err_t dataSent = esp_now_send(broadcastAddress, (uint8_t *)&calSignalSender, sizeof(calSignalSender));
}

void onDataReceived(const uint8_t *mac, const uint8_t *incomingData, int len) {
  // Uncomment this line to print "here" to the serial monitor for debugging
  // Serial.println("here");

  // Switch statement to handle different lengths of data
  switch (len) {
    // Case when the length of incoming data matches the size of imuInfoReceiver
    case sizeof(imuInfoReceiver):
      // Copy the incoming data into imuInfoReceiver structure
      memcpy(&imuInfoReceiver, incomingData, sizeof(imuInfoReceiver));
      break;

    // Case when the length matches the size of joystickReceiver
    case sizeof(joystickReceiver):
      // Copy the incoming data into joystickReceiver structure
      memcpy(&joystickReceiver, incomingData, sizeof(joystickReceiver));
      break;

    // Case when the length matches the size of pid_info_receive
    case sizeof(pid_info_receive):
      // Copy the incoming data into pid_info_receive structure
      memcpy(&pid_info_receive, incomingData, sizeof(pid_info_receive));
      // Display tuning values (functionality depends on the displayTunningValue function)
      displayTunningValue();
      break;

    // Case when the length matches the size of imuStatusReceive
    case sizeof(imuStatusReceive):
      // Copy the incoming data into imuStatusReceive structure
      memcpy(&imuStatusReceive, incomingData, sizeof(imuStatusReceive));
      break;

    // Default case for any other data length
    default:
      // Handle unexpected data length
      // Uncomment the following line to print a warning message to the serial monitor
      // Serial.println("Received data of unexpected length.");
      break;
  }
}

void sendJoystickXY(int x, int y)
{

  joystickSender.joystickx = x;
  joystickSender.joysticky = y;

  // Send data

  esp_now_send(broadcastAddress, (uint8_t *)&joystickSender, sizeof(joystickSender));
}

void tunningCommandSend(int state) {
  // Set the pitch tuning parameters in the tunningSender structure from the global variables
  tunningSender.kpPitch = kp_pitch; // Proportional gain for pitch
  tunningSender.kiPitch = ki_pitch; // Integral gain for pitch
  tunningSender.kdPitch = kd_pitch; // Derivative gain for pitch

  // Set the roll tuning parameters in the tunningSender structure from the global variables
  tunningSender.kpRoll = kp_roll;   // Proportional gain for roll
  tunningSender.kiRoll = ki_roll;   // Integral gain for roll
  tunningSender.kdRoll = kd_roll;   // Derivative gain for roll

  // Set the yaw tuning parameters in the tunningSender structure from the global variables
  tunningSender.kpYaw = kp_yaw;     // Proportional gain for yaw
  tunningSender.kiYaw = ki_yaw;     // Integral gain for yaw
  tunningSender.kdYaw = kd_yaw;     // Derivative gain for yaw

  // Set the current tuning state
  tunningSender.tunningState = state;

  // Send the tuning data using ESP-NOW
  // broadcastAddress should be a globally defined array containing the MAC address of the target device
  // tunningSender is a structure containing the tuning parameters and state
  // The data is sent as a byte array (uint8_t) and its size is determined by the size of the tunningSender structure
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