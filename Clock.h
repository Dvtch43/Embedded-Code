// Defining a Clock class to manage time values as an object, enabling the use of interrupts to modify values dynamically

#include <string>
using namespace std;

class Clock {
private:
    int hours, minutes, seconds;   // Variables to store time components
    string period;                 // Stores "AM" or "PM"

public:
    // Default constructor
    Clock();

    // Parameterized constructor to initialize clock object
    Clock(int h, int m, int s, string ap);

    // Function to update the time variables
    void set_time(int h, int m, int s, string ap);

    // Functions to retrieve time components
    int get_hour();

    int get_minute();

    int get_second();

    string get_am_pm();
};
