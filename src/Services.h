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
    Service(std::string name, bool pro, float basePrice, const date &duration);
};


enum processState {
    Scheduled,
    InProgress,
    Complete,
    Canceled
};

class Intervention {
private:

    std::string _clientId;
    std::string _collabId;
    date _startingTime;
    Service _type;
    bool _forcePro;
    processState _state;
    float _price;

public:

    Intervention(const date &appointment, Service  type, bool forcePro);

    const date *getStartingTime() const;

    const Service* getService() const;

    bool getForcePro() const;

    std::string getCollabId() const;

    std::string getClientId() const;

    void setCollabId(const std::string& collabId);

    void setClientId(const std::string &clientId);

    processState getProcessState();

    void setProcessState(processState state);

    bool conflictsWith(date start, date duration);

    float getPrice() const;

    date getEndTime() const;

};

#endif
