#include "Temperature.h"

// Default constructor initializes temperature values to defaults
Temperature::Temperature() {
    tempCelsius = 22;   // Default Celsius temperature
    tempFahrenheit = 72; // Default Fahrenheit temperature
    tempUnit = ' ';      // Default unit (uninitialized)
}

// Parameterized constructor to initialize a temperature object
Temperature::Temperature(int c, int f, char u) {
    tempCelsius = c;
    tempFahrenheit = f;
    tempUnit = u;
}

// Updates temperature values
void Temperature::set_temp(int c, int f) {
    tempCelsius = c;
    tempFahrenheit = f;
}

// Updates the temperature unit (Celsius or Fahrenheit)
void Temperature::set_unit(char u) {
    tempUnit = u;
}

// Returns the current temperature in the selected unit
int Temperature::display_temp() {
    if (tempUnit == 'C') {
        return tempCelsius;
    }
    else {
        return tempFahrenheit;
    }
}

// Returns the current unit (C or F)
char Temperature::unit_char() {
    return tempUnit;
}
