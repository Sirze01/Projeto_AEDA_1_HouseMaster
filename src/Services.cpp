#include"Services.h"


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

date date::operator+(date& d1)
{
    date service_date;
    service_date.minute = (this->minute + d1.minute) % 60;
    service_date.hour = (this->hour + d1.hour + ((this->minute + d1.minute) / 60)) % 24;
    if (((this->hour + d1.hour + ((this->minute + d1.minute) / 60)) / 24) != 0)
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


// Despite the static variables being already 0 initialized
unsigned Intervention::_idSeq = 0;

Intervention::Intervention(date appointment, Service type, bool forcePro): _appointment(appointment), _type(type),
    _forcePro(forcePro), _id(++_idSeq), _state(Scheduled) {

}

Service Intervention::getService() {
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

float Intervention::getPrice() const {
    return _price;
}

