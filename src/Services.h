#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include <stdexcept>
#include <string>
#include <sstream>
#include <iostream>
#include <utility>

#include "Date.h"
#include "Services_Constants.h"

class Service {
private:
    std::string _name;
    bool _pro;
    float _basePrice;
    Duration _duration;
public:
    Service();

    Service(std::string name, bool pro, float basePrice, Duration duration);

    virtual ~Service() = default;

    std::string getName() const;

    bool getPro() const;

    float getBasePrice() const;

    Duration getDuration() const;

    virtual float getPrice();
};

class Painting : public Service {
private:
    unsigned int _roomNumber;
public:
    Painting(std::string name, bool pro, float basePrice, const Duration &duration, unsigned int nrOfRooms = 0);

    ~Painting() override = default;

    void setRoomNumber(unsigned int number);

    unsigned int getRoomNumber() const;

    float getPrice() override;
};


enum processState {
    Active,
    Complete,
    Canceled
};

class Intervention {
private:

    std::string _clientId;
    std::string _collabId;
    Date _startingTime;
    Service *_type;
    bool _forcePro;
    processState _state;
    float _cost;

public:

    Intervention(Date start, Service *service, bool forcePro, unsigned int nrOfRooms, processState state,
                 float cost, std::string collabId, std::string clientId);

    ~Intervention();

    Date getStartingTime() const;

    Service * getService() const;

    bool getForcePro() const;

    std::string getCollabId() const;

    std::string getClientId() const;

    void setCollabId(const std::string &collabId);

    void setClientId(const std::string &clientId);

    processState getProcessState();

    void setProcessState(processState state);

    bool conflictsWith(const Date &start, const Duration &duration) const;

    void calculateCost(); // Calculates cost to the client, earnings to housemaster and collab

    float getCost() const;

    bool getPaid() const;

    Date getEndTime() const;

    void pay();

    bool isActive() const;

};

#endif
