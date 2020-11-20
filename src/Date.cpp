#include "Date.h"
#include <iostream>

/**
 * @brief default date
 */
Date::Date() = default;


/**
 * @brief Sets the date with the integer numbers provided
 * @param day sets the day
 * @param month sets the month
 * @param year sets the year
 * @param hours sets the hours
 * @param minutes sets the minutes
 */
Date::Date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes) {
    this->_day = day;
    this->_month = month;
    this->_year = year;
    this->_hours = hours;
    this->_minutes = minutes;
}

/**
 * @brief Sets the date with a given string
 * @param date the date in the form of a string
 */
Date::Date(const std::string &date) {
    std::stringstream ss(date);
    char sep{};
    // DD/MM/YYYY HH:mm
    ss >> _day >> sep >> _month >> sep >> _year >> _hours >> sep >> _minutes;
}


/**
 * @brief Searches for the number of days that a month has
 * @return The number of days of a specific month
 */
unsigned int Date::getDaysInMonth() const {
    switch (_month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear() ? 29 : 28;
        default:
            throw InvalidDate(getString() + " - this month doesn't exist!");
    }
}

/**
 * @brief Sees if a date is valid or not (year, month, day in correct range), throwing adequate exception if not
 */
void Date::checkIfValid() const {
    if (_day < 1 || _day > getDaysInMonth() || _month < 1 || _month > 12 || _hours > 23 || _minutes > 59) {
        throw InvalidDate(getString() + " isn't a valid date!");
    }
}


/**
 * @brief Constructor of the exception to be thrown when a date is invalid
 * @param error_msg the message to be shown
 */
Date::InvalidDate::InvalidDate(const std::string &error_msg) : std::invalid_argument(error_msg) {

}

/**
 * @brief Converts a date in a string. Useful to show during outputs.
 * @return A string with the date in DD/MM/YYYY HH:mm
 */
std::string Date::getString() const {
    std::ostringstream stream;
    stream << std::setfill('0');
    stream << std::setw(2) << _day << '/' << _month << '/' << std::setw(4)
           << _year << "  " << std::setw(2) << _hours << ':' << _minutes;
    return stream.str();
}


/**
 * @brief Adds two dates (or a date and a duration) together and returns the final value
 * @param d1 the other operand
 * @return A final date corresponding to the addition of two dates or a date and a duration
 */
Date Date::operator+(const Date &d1) const {
    Date resultDate(_day + d1._day, _month + d1._month, _year + d1._year,
                    (_hours + d1._hours + ((_minutes + d1._minutes) / 60)) % 24, (_minutes + d1._minutes) % 60);
    if (((_hours + d1._hours + ((_minutes + d1._minutes) / 60)) / 24) != 0) {
        resultDate._day++;
        while (resultDate._day > resultDate.getDaysInMonth()) {
            resultDate._day -= resultDate.getDaysInMonth();
            resultDate._month += 1;
            if (resultDate._month > 12) {
                resultDate._month = 1;
                resultDate._year += 1;
            }
        }
    }
    return resultDate;
}

/**
 * @brief Checks whether the two dates are the same
 * @param d2 the other operand
 * @return Whether a date is equal to another
 */
bool Date::operator==(const Date &d2) const {
    return _day != d2._day || _month != d2._month || _year != d2._year || _hours != d2._hours ||
           _minutes != d2._minutes;
}


/**Checks whether the first date is before the second
 * Compares two dates and checks if the first one is before de second.
 * \return Whether the first date is before the second one*/
bool Date::operator<(const Date &d2) const {
    if (_year != d2._year) return (_year < d2._year);
    if (_month != d2._month) return (_month < d2._month);
    if (_day != d2._day) return (_day < d2._day);
    if (_hours != d2._hours) return (_hours < d2._hours);
    if (_minutes != d2._hours) return (_minutes < d2._minutes);
    return false;
}


/**
 * @brief Checks whether the first date is after the second
 * @param d2 the other operand
 * @return Whether the first date is after the second one
 */
bool Date::operator>(const Date &d2) const {
    return (!this->operator<(d2)) && (!this->operator==(d2));
}

/**
 * @brief checks if it is a leap year
 * @return true is leap year false otherwise
 */
bool Date::isLeapYear() const {
    return (_year % 400 == 0 || (_year % 4 == 0 && _year % 100 != 0));
}

/**
 * @brief default duration
 */
Duration::Duration() = default;


/**
 * @brief Sets the duration with the integer numbers provided
 * @param hours sets the hours
 * @param minutes sets the minutes
 */
Duration::Duration(unsigned int hours, unsigned int minutes) {
    _hours = hours;
    _minutes = minutes;
}

/**
 * @brief Sets the duration with a given string
 * @param duration sets the duration
 */
Duration::Duration(const std::string &duration) {
    std::stringstream ss(duration); //HH:mm
    char sep{};
    ss >> _hours >> sep >> _minutes;
}


/**
 * @brief Uses the format: HHhmm. Useful to show during outputs
 * @return A string with the corresponding duration
 */
std::string Duration::getString() const {
    std::ostringstream stream;
    stream << std::setfill('0') << std::setw(2) << _hours << "h" << _minutes;
    return stream.str();
}


/**
 * @brief Constructor of the exception to be thrown when a duration is invalid
 * @param error_msg the message to be shown
 */
Duration::InvalidDuration::InvalidDuration(const std::string &error_msg) : std::invalid_argument(error_msg) {}


/**
 * @brief Sees if a duration is valid or not (hours, minutes in correct range), throwing adequate exception if not
 */
void Duration::checkIfValid() const {
    if (_hours > 23 || _minutes > 59) {
        throw InvalidDuration(getString() + " isn't a valid duration!");
    }
}
