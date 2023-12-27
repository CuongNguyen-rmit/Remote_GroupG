
#define LR_PIN 35 // Pin 33 attached to Left Right pin
#define UD_PIN 32
#define SW_PIN 33

extern int lastLRValue;
extern int lastUDValue;
extern const int threshold_joystick; // Threshold for change detection

void joystickInit();