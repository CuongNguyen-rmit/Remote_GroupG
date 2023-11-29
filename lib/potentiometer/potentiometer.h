// ================================================================
// Variable declaration
// ================================================================
#define MAX_SIGNAL 2000 // Parameter required for the ESC definition
#define MIN_SIGNAL 1000 // Parameter required for the ESC definition
#define POT_PIN 34      // Pin 35 attached to the potentiometer

// ================================================================
// Function declaration
// ================================================================
void SerialDataPrintPotentiometer(); // Function to print data on the serial monitor
void mapPotentiometerValue();