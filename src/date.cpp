#include "date.h"
#include <iostream>
/**Sets the default date*/
date::date() = default;

/**Sets the date with the integer numbers provided
 * Attributes the specified values to the members of date struct
 * \param day sets the day
 * \param month sets the month
 * \param year sets the year
 * \param hours sets the hours
 * \param minutes sets the minutes*/
date::date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->hours = hours;
    this->minutes = minutes;
}

/**Sets the date with a given string
 * \param date sets the date*/
date::date(const std::string &date) {
    std::stringstream ss(date);
    char sep{};
    // DD/MM/YYYY HH:mm
    ss >> day >> sep >> month >> sep >> year >> hours >> sep >> minutes;
}

/**Searches for the number of days that a month has
 * It has attention to leap years
 * \return The number of days of a specific month*/
unsigned int date::getDaysInMonth() const {
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) {
        return 31;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else if (month == 2) {
        if (year % 4 == 0) {
            if (year % 100 == 0) {
                if (year % 400 == 0) {
                    return 29;
                } else return 28;
            } else return 29;
        } else return 28;
    } else throw InvalidDate(dateToStr() + " - this month doesn't exist!");
}


/** Sees if a date is valid or not
     * Does not allow days that don´t belong to the interval [1, daysInMonth], months that don´t belong
     * to the interval [1, 12], hours that are over 24 and minutes thar are over 59
     * \param throwExcept is used only when we want to throw an exception. On the
     * normal code flow it is unnecessary since throwing the exception will stop
     * the program, so by default we use it as false
     * \return Whether the duration is valid or invalid*/
bool date::isValidDate(bool throwExcept) const {
    if (day < 1 || day > getDaysInMonth() || month < 1 || month > 12 || hours > 23 || minutes > 59) {
        if(throwExcept)
            throw InvalidDate(dateToStr() + " isn't a valid date!");
        return false;
    }
    return true;
}


/**Sets an invalid date
 * Used to throw an exception*/
date::InvalidDate::InvalidDate(const std::string &error_msg) : std::invalid_argument(error_msg) {}


/**Converts a date in a string
 * Uses the format: DD/MM/YYYY HH:mm. Useful to show during outputs
 * \return A string with the corresponding date*/
std::string date::dateToStr() const {
    std::ostringstream stream;
    stream << std::setfill('0');
    stream << std::setw(2) << day << '/' << month << '/' << std::setw(4)
    << year << "  " << std::setw(2) << hours << ':' << minutes;
    return stream.str();
}


/**Adds two dates (or a date and a duration) and returns the final value
 * Cases where the year, month, day and hour ends are considered. It is also useful to add a date and a duration.
 * \return A final date corresponding to the addition of two dates or a date and a duration*/
date date::operator+(const date &d1) const {
    date service_date;
    service_date.minutes = (this->minutes + d1.minutes) % 60;
    service_date.hours = (this->hours + d1.hours + ((this->minutes + d1.minutes) / 60)) % 24;
    service_date.day = this->day + d1.day;
    service_date.month = this->month + d1.month;
    service_date.year = this->year + d1.year;
    if (((this->hours + d1.hours + ((this->minutes + d1.minutes) / 60)) / 24) != 0) {
        service_date.day += 1;
        while (service_date.day > service_date.getDaysInMonth()) {
            service_date.day -= service_date.getDaysInMonth();
            service_date.month += 1;
            if (service_date.month > 12) {
                service_date.month = 1;
                service_date.year += 1;
            }
        }
    }
    return service_date;
}


/**Checks whether the two dates are the same
 * It is also useful to compare durations.
 * \return Whether a date is equal to another*/
bool date::operator==(const date &d2) const{
    if (day != d2.day || month != d2.month || year != d2.year || hours != d2.hours || minutes != d2.minutes)
        return false;
    return true;
}


/**Checks whether the first date is before the second
 * Compares two dates and checks if the first one is before de second.
 * \return Whether the first date is before the second one*/
bool date::operator<(const date &d2) const
{
    if (year < d2.year) {return true;}
    else if (year > d2.year) {return false;}
    else            //same year
    {if (month < d2.month) {return true;}
        else if (month > d2.month){return false;}
        else        //same month
        {
            if (day < d2.day) {return true;}
            else if (day > d2.day) {return false;}
            else    //same day
            {
                if (hours < d2.hours) {return true;}
                else if (hours > d2.hours) {return false;}
                else        //same hour
                {
                    if (minutes < d2.minutes) {return true;}
                    else    //if (minutes >= d2.minutes)
                    {return false;}
                }
            }
        }
    }
}


/**Checks whether the first date is after the second
 * Compares two dates and checks if the first one is after de second.
 * \return Whether the first date is after the second one*/
bool date::operator>(const date &d2) const
{
    if (year > d2.year)
    {return true;}
    else if (year < d2.year)
    {return false;}
    else            //same year
    {
        if (month > d2.month) {return true;}
        else if (month < d2.month) {return false;}
        else        //same month
        {
            if (day > d2.day) {return true;}
            else if (day < d2.day) {return false;}
            else    //same day
            {
                if (hours > d2.hours) {return true;}
                else if (hours < d2.hours) {return false;}
                else        //same hour
                {
                    if (minutes > d2.minutes) {return true;}
                    else    //if (minutes <= d2.minutes)
                    {return false;}
                }
            }
        }
    }
}


/**Sets the default duration*/
duration::duration() = default;

/**Sets the duration with the integer numbers provided
 * Attributes the specified values to the members of duration struct
 * \param hours sets the hours
 * \param minutes sets the minutes*/
duration::duration(unsigned int hours, unsigned int minutes)
{
    this->hours = hours;
    this->minutes = minutes;
}

/**Sets the duration with a given string
 * \param duration sets the duration*/
duration::duration(const std::string &duration) {
    std::stringstream ss(duration);
    //HH:mm
    char sep{};
    ss >> hours >> sep >> minutes;
}

/**Converts a duration in a string
 * Uses the format: HHhmm. Useful to show during outputs
 * \return A string with the corresponding duration*/
std::string duration::durationToStr() const
{
    std::ostringstream stream;
    stream << std::setfill('0') <<std::setw(2) << hours << "h" << minutes;
    return stream.str();
}


/**Sets an invalid duration
 * Used to throw an exception*/
duration::InvalidDuration::InvalidDuration(const std::string &error_msg) : std::invalid_argument(error_msg) {}

/** Sees if a duration is valid or not
     * Does not allow hours that are over 24 and minutes thar are over 59
     * \param throwExcept is used only when we want to throw an exception. On the
     * normal code flow it is unnecessary since throwing the exception will stop
     * the program, so by default we use it as false
     * \return Whether the duration is valid or invalid*/
bool duration::isValidDuration(bool throwExcept) const {
    if (hours > 23 || minutes > 59) {
        if(throwExcept)
            throw InvalidDuration(durationToStr() + " isn't a valid date!");
        return false;
    }
    return true;
}
