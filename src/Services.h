#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include<stdexcept>
#include<string>
#include<sstream>
#include<iostream>

struct date {
    date();
    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes, int valid  = 1);
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int hours;
    unsigned int minutes;
    int getDaysInMonth();
    void setDate(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes);
    date operator+(date& d1);
    bool operator==(const date &d2) const;
    std::string dateToStr();
    class InvalidDate;
};

class date::InvalidDate : public std::invalid_argument{
public:
    InvalidDate(const std::string & error_msg);
};

struct Service {
    bool pro;
    std::string name;
    float basePrice;
};


enum processState {
    WaitingPayment,
    Scheduled,
    InProgress,
    Concluded
};

class Intervention {
private:
    unsigned int _id;
    date _appointment;
    Service _type;
    bool _forcePro;
    processState _state;
    float _price;

public:
    static unsigned int _idSeq;

    Intervention(date appointment, Service type, bool forcePro);

    date* getAppointment();

    Service* getService();

    processState getProcessState();

    float getPrice() const;
};

#endif
