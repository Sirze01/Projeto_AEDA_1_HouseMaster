#include"Services.h"

date::InvalidDate::InvalidDate(const std::string& error_msg): std::invalid_argument(error_msg) {}

int date::getDaysInMonth()
{
    if (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12)
    {
        return 31;
    }
    else if (month == 4 || month == 6 || month == 9 || month == 11)
    {
        return 30;
    }
    else if (month == 2)
    {
        if (year % 4 == 0)
        {
            if (year % 100 == 0)
            {
                if (year % 400 == 0)
                {
                    return 29;
                }
                else return 28;
            }
            else return 29;
        }
        else return 28;
    }
    else return -1;     // exception
}

date::date(){}

date::date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes, int valid) {
    if(valid){
       setDate(day, month, year, hours, minutes);
    }
    else{
        this -> day = day;
        this -> month = month;
        this -> year = year;
        this -> hours = hours;
        this -> minutes = minutes;
    }
}

void date::setDate(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes) {
    this -> day = day;
    this -> month = month;
    this -> year = year;
    this -> hours = hours;
    this -> minutes = minutes;

    if(day < 1 || day > getDaysInMonth() + 1)
        throw InvalidDate(dateToStr() + " isn't a valid date!");

    else if(month < 1 || month > 12)
        throw InvalidDate(dateToStr() + " isn't a valid date!");

    else if(hours > 23)
        throw InvalidDate(dateToStr() + " isn't a valid date!");

    else if (minutes > 59)
        throw InvalidDate(dateToStr() + " isn't a valid date!");
}

bool date::operator==(const date &d2) const{
    if (day != d2.day || month != d2.month || year != d2.year || hours != d2.hours || minutes != d2.minutes)
        return 0;
    return 1;
}

date date::operator+(date& d1)
{
    date service_date;
    service_date.minutes = (this->minutes + d1.minutes) % 60;
    service_date.hours = (this->hours + d1.hours + ((this->minutes + d1.minutes) / 60)) % 24;
    if (((this->hours + d1.hours + ((this->minutes + d1.minutes) / 60)) / 24) != 0)
    {
        service_date.month = d1.month;
        service_date.year = d1.year;
        service_date.day = d1.day + 1;
        while (service_date.day > service_date.getDaysInMonth())
        {
            service_date.day -= service_date.getDaysInMonth();
            service_date.month += 1;
            if (service_date.month > 12)
            {
                service_date.month = 1;
                service_date.year += 1;
            }
        }
    }
    else{
        service_date.day = d1.day;
        service_date.month = d1.month;
        service_date.year = d1.year;
    }

    return service_date;
}

std::string date::dateToStr() {
    std::ostringstream stream;
    stream << day << '/' << month << '/' << year << "  " << hours << ':' << minutes;
    return stream.str();
}


// Despite the static variables being already 0 initialized
unsigned Intervention::_idSeq = 0;

Intervention::Intervention(date appointment, Service type, bool forcePro): _appointment(appointment), _type(type),
    _forcePro(forcePro), _id(++_idSeq), _state(Scheduled) {}

Service Intervention::getService() {
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

float Intervention::getPrice() const {
    return _price;
}

