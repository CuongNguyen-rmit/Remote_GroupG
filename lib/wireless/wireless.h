extern uint8_t broadcastAddress[6];

// Define the incoming data, RECEIVED into this board
typedef struct struct_msg_Receive
{
    int Receive_PotValue;
} struct_msg_Receive;

// Define the outgoing data, SENT out from this board
typedef struct struct_msg_Send
{
    int joystickX; // Value for the X-axis of the joystick
    int joystickY; // Value for the Y-axis of the joystick
} struct_msg_Send;

// ******************************************
// Function declaration
// ******************************************
float floatMap(float, float, float, float, float);
void espnow_initialize();