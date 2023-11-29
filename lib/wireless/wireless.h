extern uint8_t broadcastAddress[6];

// Define the outgoing data, SENT out from this board
typedef struct struct_msg_Send
{
    int joystickX; // Value for the X-axis of the joystick
    int joystickY; // Value for the Y-axis of the joystick
} struct_msg_Send;

typedef struct struct_msg_Send_JTButton
{
    bool pressed;
} struct_msg_Send_JTButton;

typedef struct struct_msg_Send_POT
{
    int value;
} struct_msg_Send_POT;

// ******************************************
// Function declaration
// ******************************************
float floatMap(float, float, float, float, float);
void espnow_initialize();