#pragma once

#include<string>

struct date{
    unsigned int day;
    unsigned int month;
    unsigned int year;
    unsigned int hour;
    unsigned int minute;
};

struct servicesType{
    bool pro;
    std::string type;
};

enum processState{
    WaitingPayment,
    Agended,
    InProgress,
    Concluded
};

class Intervention{
private:
    unsigned int _id;
    date _appointment;
    servicesType _type;
    bool _forcePro;
    processState _state;
    float _price;

public:
    static unsigned int _idSeq;
    Intervention(date appointment, servicesType type, bool forcePro);
    servicesType getType();
    processState getProcessState();
    float getPrice();

};
