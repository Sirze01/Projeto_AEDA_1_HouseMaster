#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include<stdexcept>
#include<string>
#include<sstream>
#include<iostream>

#include "date.h"

struct Service {
    std::string name;
    bool pro{};
    float basePrice{};
    date duration;
    Service() = default;
    Service(const std::string &name, bool pro, float basePrice, const date &duration);
};


enum processState {
    WaitingPayment,
    Scheduled,
    InProgress,
    Concluded
};

class Intervention {
private:
    //unsigned int _id;
    std::string _clientId;
    std::string _collabId;
    Service _type;
    date _startingTime;
    bool _forcePro;
    float _price;
    processState _state;

public:
    static unsigned int _idSeq;

    Intervention(date appointment, Service type, bool forcePro);

    const date *getStartingTime() const;

    Service* getService();

    bool getForcePro() const;

    std::string getCollabId() const;

    std::string getClientId() const;

    void setCollabId(unsigned int collabId);

    processState getProcessState();

    bool conflictsWith(date start, date duration);

    float getPrice() const;

    date getEndTime() const;
};

#endif
