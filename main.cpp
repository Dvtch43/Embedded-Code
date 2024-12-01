#include "mbed.h"
#include "TextLCD.h"
#include <string>
#include "Clock.h"          // Header file for Clock class to manage time
#include "Temperature.h"    // Header file for Temperature class to manage temperature

// To-do: Implement interrupts for set mode and temperature unit toggling
// Note: Consider encapsulating time and temperature arrays within their respective Clock and Temperature classes

// Instantiate the LCD display (RS, E, D4-D7 pins are specified)
TextLCD display(PA_0, PA_1, PA_4, PB_0, PC_1, PC_0);

// Function declarations for modes and utilities
void defaultMode();                            // Handles incrementing time and toggling AM/PM
void configureTime();                          // User input to set time values and AM/PM
int getInput(int lowerBound, int upperBound);  // Gets input within a valid range
int convertToInt(string input);               // Converts string input into integer format

// Keypad GPIO initialization
DigitalOut row1(PA_6);
DigitalOut row2(PA_7);
DigitalOut row3(PB_6);
DigitalOut row4(PC_7);

DigitalIn col1(PA_9, PullUp);
DigitalIn col2(PA_8, PullUp);
DigitalIn col3(PB_10, PullUp);
DigitalIn col4(PB_4, PullUp);

// External interrupt on the '*' key of the keypad
InterruptIn modeSwitch(PA_10);
void enterSetMode();       // ISR to switch to set mode

// Keypad scan functions
int scanColumn();          // Determines the pressed column
char getKeypadInput();     // Reads character from keypad

// Temperature sensor initialization
AnalogIn tempSensor(D13);
InterruptIn tempButton(PC_13);  // Push-button interrupt for toggling temperature unit
void readTemperature();         // Reads and updates temperature values
void toggleUnit();              // Changes the temperature unit

// Create Clock and Temperature objects with default values
Clock systemClock(11, 59, 55, "PM");
Temperature currentTemperature(22, 72, 'C');

int main() {
    tempButton.fall(&toggleUnit);   // Assign ISR for temperature unit toggling
    modeSwitch.fall(&enterSetMode); // Assign ISR for set mode activation

    while (true) {
        display.cls();
        // Display time and temperature in the format hh:mm:ss AM/PM Temp°
        display.printf("%02d:%02d:%02d %s %d%c", systemClock.hour(), systemClock.minute(),
            systemClock.second(), systemClock.am_pm(),
            currentTemperature.display_temp(), currentTemperature.unit_char());
        readTemperature();          // Read and update temperature
        defaultMode();              // Increment time by one second
        wait_ms(975);               // Pause for one second
    }
}

// Default mode to manage time increment and AM/PM toggle
void defaultMode() {
    int hours = systemClock.hour();
    int minutes = systemClock.minute();
    int seconds = systemClock.second();
    string period = systemClock.am_pm();

    // Increment seconds, rollover to minutes at 60
    seconds = (seconds + 1) % 60;

    if (seconds == 0) {
        minutes = (minutes + 1) % 60;

        // Increment hours, rollover to 12 and toggle AM/PM when necessary
        if (minutes == 0 && hours != 11) {
            hours = (hours + 1) % 12;
        }
        else if (minutes == 0 && hours == 11) {
            hours = 12;
            period = (period == "AM") ? "PM" : "AM";
        }
    }

    systemClock.set_time(hours, minutes, seconds, period);
}

// Interrupt routine to activate set mode
void enterSetMode() {
    if (getKeypadInput() == '*') {  // Trigger only on '*'
        wait_ms(300);               // Debounce keypad input
        configureTime();            // Enter time configuration mode
    }
}

// Set mode to allow user to configure time
void configureTime() {
    int hours, minutes, isPM, seconds = 0;
    string period = "";

    // Get hour input
    display.cls();
    display.printf("Enter Hour (1-12): ");
    hours = getInput(1, 12);
    wait_ms(300);

    // Get minute input
    display.cls();
    display.printf("Enter Minutes (0-59): ");
    minutes = getInput(0, 59);
    wait_ms(300);

    // Get AM/PM input
    display.cls();
    display.printf("AM (0) or PM (1): ");
    isPM = getInput(0, 1);
    period = (isPM == 0) ? "AM" : "PM";

    systemClock.set_time(hours, minutes, seconds, period); // Update clock
}

// Function to read and validate user input
int getInput(int lowerBound, int upperBound) {
    string userInput;
    int numericValue;
    bool isValid = false;

    while (!isValid) {
        userInput = "";
        while (getKeypadInput() != '#') {  // '#' serves as the enter key
            char inputChar = getKeypadInput();
            if (inputChar != '_') {
                display.printf("%c", inputChar);  // Display input
                userInput += inputChar;          // Record input
                wait_ms(300);                    // Debounce
            }
        }
        numericValue = convertToInt(userInput);
        if (numericValue >= lowerBound && numericValue <= upperBound) {
            isValid = true;
        }
        else {
            display.cls();
            display.printf("Invalid input. Retry.");
            wait_ms(300);
        }
    }
    return numericValue;
}

// Convert string to integer
int convertToInt(string input) {
    char buffer[10];
    string validStrings[60];

    for (int i = 0; i < 60; i++) {
        sprintf(buffer, "%d", i);
        validStrings[i] = buffer;
    }

    for (int i = 0; i < 60; i++) {
        if (input.compare(validStrings[i]) == 0) {
            return i;
        }
    }
    return 61;  // Return invalid value if conversion fails
}

// Read temperature from sensor and update object
void readTemperature() {
    float voltage = tempSensor.read();
    float tempCelsius = voltage / 0.01;
    float tempFahrenheit = (tempCelsius * 9.0 / 5.0) + 32;

    currentTemperature.set_temp(tempCelsius, tempFahrenheit);
}

// Toggle temperature unit between Celsius and Fahrenheit
void toggleUnit() {
    char currentUnit = currentTemperature.unit_char();
    currentTemperature.set_unit((currentUnit == 'C') ? 'F' : 'C');
}

// Scan columns to detect which key is pressed
int scanColumn() {
    if (col1 == 0) return 0;
    if (col2 == 0) return 1;
    if (col3 == 0) return 2;
    if (col4 == 0) return 3;
    return 255;
}

// Get character input from keypad
char getKeypadInput() {
    char keyMap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    for (int i = 1; i <= 4; i++) {
        row1 = row2 = row3 = row4 = 1;
        switch (i) {
        case 1: row1 = 0; break;
        case 2: row2 = 0; break;
        case 3: row3 = 0; break;
        case 4: row4 = 0; break;
        }

        int col = scanColumn();
        if (col != 255) return keyMap[i - 1][col];
    }
    return '_';
}
