#pragma once

#include<string>
#include"Individuals.h"

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
    static unsigned int _idSeq;
    unsigned int _id;
    date _appointment;
    servicesType _type;
    bool _forcePro;
    Collaborator* _assigned;
    processState state;
    float price;


public:
    Intervention(date appointment, servicesType type, bool forcePro);
    servicesType getType();
    processState getProcessState();
    float getPrice();

};