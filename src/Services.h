#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>

#include "date.h"
#include "Services_Constants.h"

class Service {
private:
    std::string _name;
    bool _pro;
    float _basePrice;
    date _duration;
public:
    Service() = default;
    Service(std::string name, bool pro, float basePrice, const date &duration);
    std::string getName() const;
    bool getPro() const;
    float getBasePrice() const;
    date getDuration() const;
    virtual float calculatePrice();
};

class Painting : public Service{
private:
    int _roomNumber;
public:
    Painting(std::string name, bool pro, float basePrice, const date &duration);
    void setRoomNumber(int number);
    float calculatePrice() override;
};


enum processState {
    Scheduled,
    PaymentComplete,
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
    float _cost;

public:

    Intervention(const date &appointment, Service type, bool forcePro);

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

    void calculateCost(); // Calculates cost to the client, earnings to housemaster and collab

    float getCost() const;

    date getEndTime() const;

};

#endif
