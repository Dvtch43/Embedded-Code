// Defining a Temperature class to manage temperature values as an object,
// enabling the use of interrupts for dynamic updates.

#include <string>
using namespace std;

class Temperature {
private:
    int tempCelsius, tempFahrenheit; // Variables to store temperatures in Celsius and Fahrenheit
    char tempUnit;                  // Character to store the unit ('C' or 'F')

public:
    // Default constructor initializes default temperature values
    Temperature();

    // Parameterized constructor to initialize a temperature object
    Temperature(int c, int f, char u);

    // Updates temperature values
    void set_temp(int c, int f);

    // Updates the temperature unit ('C' or 'F')
    void set_unit(char u);

    // Returns the current temperature in the selected unit
    int display_temp();

    // Returns the current unit ('C' or 'F')
    char unit_char();
};
