#include <tunning.h>

double kp_pitch = 0.95, kd_pitch = 0.013, ki_pitch = 0.015;
double kp_roll = 1.25, kd_roll = 0.006, ki_roll = 0.015;
double kp_yaw = 0.75, kd_yaw = 0.00, ki_yaw = 0.00;
int tunning_state;

void menuPrompt()
{
    Serial.println("Select the axis for PID tuning:");
    Serial.println("1. Pitch");
    Serial.println("2. Roll");
    Serial.println("3. Yaw");
    Serial.println("4. Use the init tunning and exit");
    Serial.println("5. Exit");

    while (Serial.available() == 0)
        ;

    int choice = Serial.parseInt(); // Read user choice
    Serial.print("choice is: ");
    Serial.println(choice);
    int pidFlag = 0;
    switch (choice)
    {
    case 1:
        // PID tuning for Pitch

        tunningMenu(PITCH_FLAG);
        tunning_state = 1;
        break;
    case 2:
        // PID tuning for Roll
        tunningMenu(ROLL_FLAG);
        tunning_state = 1;
        break;
    case 3:
        // PID tuning for Yaw
        tunningMenu(YAW_FLAG);
        tunning_state = 1;
        break;
    case 4:
        tunning_state = 0;
        break;
    case 5:

        break;
    default:
        Serial.println("Invalid choice. Please select 1, 2, or 3.");
        break;
    }
    Serial.println("=======================================");
    Serial.println("Press any key to exit the menu");
    waitForKeyStroke(); // Wait for user input
}

double extractNumber(const char *input)
{
    char numberPart[16];
    int j = 0;

    for (int i = 1; input[i] != '\0' && j < 15; ++i)
    {
        if ((input[i] >= '0' && input[i] <= '9') || input[i] == '.' || input[i] == '-')
        {
            numberPart[j++] = input[i];
        }
    }
    numberPart[j] = '\0';

    return atof(numberPart);
}

void readTunningValue(int flag)
{
    while (Serial.available() < 7)
        ; // wait for input 2 characters
    if (Serial.available())
    {
        char inputStr[32];
        int i = 0;
        while (Serial.available() && i < 31)
        {
            char c = Serial.read();
            if (c == '\n' || c == '\r')
            {
                break; // End of input
            }
            inputStr[i++] = c;
            delay(2); // Small delay to allow buffer to fill
        }
        inputStr[i] = '\0';
        double value = extractNumber(inputStr);

        // Check if the input is valid for Kp tuning
        switch (inputStr[0])
        {
        case 'p':
            if (flag == PITCH_FLAG)
            {
                kp_pitch = value;
            }
            else if (flag == ROLL_FLAG)
            {
                kp_roll = value;
            }
            else if (flag == YAW_FLAG)
            {
                kp_yaw = value;
            }
            break;
        case 'i':
            if (flag == PITCH_FLAG)
            {
                ki_pitch = value;
            }
            else if (flag == ROLL_FLAG)
            {
                ki_roll = value;
            }
            else if (flag == YAW_FLAG)
            {
                ki_yaw = value;
            }
            break;
        case 'd':
            if (flag == PITCH_FLAG)
            {
                kd_pitch = value;
            }
            else if (flag == ROLL_FLAG)
            {
                kd_roll = value;
            }
            else if (flag == YAW_FLAG)
            {
                kd_yaw = value;
            }
            break;
        }
    }
}

void tunningMenu(int flag)
{
    while (1)
    {
        Serial.println("\n\n====================================");
        Serial.println("|          Tuning PID Axis          |");
        Serial.println("|----------------------------------|");
        Serial.println("| To select the constant to tune:  |");
        Serial.println("| 1. Type the command as follows:  |");
        Serial.println("|    Ka                            |");
        Serial.println("|----------------------------------|");
        Serial.println("| K = PID constant (p, i, d)       |");
        Serial.println("| a = Value to tune (1,2,3,...)    |");
        Serial.println("|----------------------------------|");
        Serial.println("| Example:                         |");
        Serial.println("| Tune Kp to 3: Type 'p3'          |");
        Serial.println("====================================");
        readTunningValue(flag); // this function logically set the tunning
        // Apply the new settings to the Pitch axis control
        // UpdatePIDController(Kp_pitch, Ki_pitch, Kd_pitch);

        Serial.println("Would you like to tune another constant? (ye/no)");
        while (Serial.available() < 2)
            ;
        String response = Serial.readStringUntil('\n');
        if (response.equalsIgnoreCase("no"))
        {
            // Exit or return to the main menu
            break;
        }
        else
        {
            continue;
        }
    }
    menuPrompt(); // when user presses the exit key in the menuprompt, it will exit this function
}

int welcomeMenu()
{
    // Displaying the menu with a frame
    Serial.println("***********************");
    Serial.println("*    PID Tuning Menu  *");
    Serial.println("*---------------------*");
    Serial.println("* Do you want to tune *");
    Serial.println("* the PID? (Y/N):     *");
    Serial.println("***********************");

    // Check if data is available to read
    while (Serial.available() == 0)
        ;
    if (Serial.available() > 0)
    {
        // Read the incoming byte
        char choice = Serial.read();

        // Clear the Serial buffer
        while (Serial.available() > 0)
            Serial.read();

        // Converting to uppercase in case the user enters lowercase
        choice = toupper(choice);

        // Processing user choice
        if (choice == 'Y')
        {
            Serial.println("You have chosen to tune the PID.\n***************\n");
            return 1;
            // Here, you can add the code or function call for PID tuning.

            // End or reset the program as needed
        }
        else if (choice == 'N')
        {
            Serial.println("You have chosen not to tune the PID.\n***************\n");
            return 0;
            // Here, you can add any alternative action.

            // End or reset the program as needed
        }
        else
        {
            Serial.println("Invalid choice entered. Please enter Y or N.");
            return 1;
        }
    }
    delay(100);
    return 0;
}
