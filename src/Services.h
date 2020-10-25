#pragma once

#include<string>

struct date {
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int hour;
    unsigned int minute;

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


    Service getService();

    processState getProcessState();

    float getPrice() const;

};
