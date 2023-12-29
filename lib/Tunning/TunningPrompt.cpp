#include "./TunningPrompt.h"

double kp_pitch, kd_pitch, ki_pitch;
double kp_roll, kd_roll, ki_roll;
double kp_yaw, kd_yaw, ki_yaw;


void menuPrompt() {
    Serial.println("Select the axis for PID tuning:");
    Serial.println("1. Pitch");
    Serial.println("2. Roll");
    Serial.println("3. Yaw");

    waitForKeyStroke(); // Wait for user input

    int choice = Serial.parseInt(); // Read user choice
    int pidFlag = 0;
    switch (choice) {
        case 1:
            // PID tuning for Pitch
            pitchTunning(PITCH_FLAG);
            break;
        case 2:
            // PID tuning for Roll
            pitchTunning(ROLL_FLAG);
            break;
        case 3:
            // PID tuning for Yaw
            pitchTunning(YAW_FLAG);
            break;
        default:
            Serial.println("Invalid choice. Please select 1, 2, or 3.");
            break;
    }
    Serial.println("=======================================");
    Serial.println("Press any key to exit the menu");
    waitForKeyStroke(); // Wait for user input
}

double extractNumber(const char* input) {
    char numberPart[16];
    int j = 0;

    for (int i = 1; input[i] != '\0' && j < 15; ++i) {
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.' || input[i] == '-') {
            numberPart[j++] = input[i];
        }
    }
    numberPart[j] = '\0';

    return atof(numberPart);
}

void setTunning(int flag) {
    if (Serial.available()) {
        char inputStr[32];
        int i = 0;
        while (Serial.available() && i < 31) {
            char c = Serial.read();
            if (c == '\n' || c == '\r') {
                break; // End of input
            }
            inputStr[i++] = c;
            delay(2); // Small delay to allow buffer to fill
        }
        inputStr[i] = '\0';
        double value = extractNumber(inputStr);

        // Check if the input is valid for Kp tuning
        switch(inputStr[0]) {
            case 'p':
                if(flag == PITCH_FLAG) {
                    kp_pitch = value;
                }
                else if(flag == ROLL_FLAG) {
                    kp_roll = value;
                }
                else if(flag == YAW_FLAG) {
                    kp_yaw = value;
                }
                break;
            case 'i':
                if(flag == PITCH_FLAG) {
                    ki_pitch = value;
                }
                else if(flag == ROLL_FLAG) {
                    ki_roll = value;
                }
                else if(flag == YAW_FLAG) {
                    ki_yaw = value;
                }
                break;
            case 'd':
                if(flag == PITCH_FLAG) {
                    kd_pitch = value;
                }
                else if(flag == ROLL_FLAG) {
                    kd_roll = value;
                }
                else if(flag == YAW_FLAG) {
                    kd_yaw = value;
                }
                break;
        }
        // Prompt for next input
        Serial.println("\nEnter next PID tuning command:");
    }
}

void pitchTunning(int flag) {
    while(1) {
        Serial.println("\nTuning PID Axis:");
        Serial.println("To select the constant to tune.\nPlease type the command following this format: Ka");
        Serial.println("==========================");
        Serial.println("K is the pid constant u must type the char included here: (p,i,d)");
        Serial.println("a is the value u want to tune that constant: 1,2,3,....");
        Serial.println("Example: Tunning kp to 3: p3.");
        Serial1.println("==============================");
        setTunning(flag); // this function logically set the tunning
        // Apply the new settings to the Pitch axis control
        // UpdatePIDController(Kp_pitch, Ki_pitch, Kd_pitch);

        Serial.println("Would you like to tune another constant? (yes/no)");
        while (!Serial.available());
        String response = Serial.readStringUntil('\n');
        if (response.equalsIgnoreCase("no")) {
            // Exit or return to the main menu
            break;
        }
        else {
            continue;
        }
    }
    menuPrompt(); // when user presses the exit key in the menuprompt, it will exit this function
}