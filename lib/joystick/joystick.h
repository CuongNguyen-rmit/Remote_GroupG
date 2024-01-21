
#define LR_PIN 36 // Pin 33 attached to Left Right pin
#define UD_PIN 39

extern int lastLRValue;
extern int lastUDValue;
extern int threshold_joystick; // Threshold for change detection

void joystickInit();
