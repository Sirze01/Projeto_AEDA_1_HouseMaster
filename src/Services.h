#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include<stdexcept>
#include<string>
#include<sstream>
#include<iostream>

struct date {
    date();

    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes,
         int valid = 1);

    unsigned int day{};
    unsigned int month{};
    unsigned int year{};
    unsigned int hours{};
    unsigned int minutes{};

    int getDaysInMonth() const;

    void setDate(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes);

    date operator+(const date &d1) const;

    std::string dateToStr() const;

    class InvalidDate;
};

class date::InvalidDate : public std::invalid_argument {
public:
    explicit InvalidDate(const std::string &error_msg);
};

struct Service {
    bool pro;
    std::string name;
    float basePrice;
    date duration;
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
    date _startingTime;
    Service _type;
    bool _forcePro;
    processState _state;
    float _price;

public:
    static unsigned int _idSeq;

    Intervention(date appointment, Service type, bool forcePro);

    const date &getStartingTime() const;

    Service getService();

    processState getProcessState();

    float getPrice() const;
};

#endif
