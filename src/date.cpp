#include "date.h"

date::date() = default;

date::date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes,
           bool valid) {
    this->day = day;
    this->month = month;
    this->year = year;
    this->hours = hours;
    this->minutes = minutes;
    if(valid){
        isValidDate(true);
    }

}

int date::getDaysInMonth() const {
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
    } else return -1;     // exception
}

bool date::isValidDate(bool throwExcept) {
    if (day < 1 || day > getDaysInMonth() || month < 1 || month > 12 || hours > 23 || minutes > 59) {
        if(throwExcept)
            throw InvalidDate(dateToStr() + " isn't a valid date!");
        return false;
    }
    return true;
}

date::InvalidDate::InvalidDate(const std::string &error_msg) : std::invalid_argument(error_msg) {}

std::string date::dateToStr() const {
    std::ostringstream stream;
    stream << day << '/' << month << '/' << year << "  " << hours << ':' << minutes;
    return stream.str();
}

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

bool date::operator==(const date &d2) const{
    if (day != d2.day || month != d2.month || year != d2.year || hours != d2.hours || minutes != d2.minutes)
        return 0;
    return 1;
}

bool date::operator<(const date &d2) const
{
    if (year < d2.year) {return true;}
    else if (year > d2.year) {return false;}
    else            //same year
    {if (month < d2.month) {return true;}
        else if (month > d2.month) {return false;}
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

void date::readDuration(const std::string &duration) {
    std::stringstream ss(duration);
    char sep{};
    ss >> hours >> sep >> minutes;
}
