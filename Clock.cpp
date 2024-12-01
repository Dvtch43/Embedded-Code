#include "Clock.h"
#include <iostream>

// Default constructor initializes clock to default time values
Clock::Clock() {
    hours = 11;         // Default hour
    minutes = 59;       // Default minute
    seconds = 55;       // Default second
    period = "PM";      // Default period (AM/PM)
}

// Parameterized constructor to initialize clock object with specified time values
Clock::Clock(int h, int m, int s, string ap) {
    hours = h;
    minutes = m;
    seconds = s;
    period = ap;
}

// Updates the time variables in the clock object
void Clock::set_time(int h, int m, int s, string ap) {
    hours = h;
    minutes = m;
    seconds = s;
    period = ap;
}

// Returns the hour component of the time
int Clock::get_hour() {
    return hours;
}

// Returns the minute component of the time
int Clock::get_minute() {
    return minutes;
}

// Returns the second component of the time
int Clock::get_second() {
    return seconds;
}

// Returns the period (AM/PM) of the time
string Clock::get_am_pm() {
    return period;
}
