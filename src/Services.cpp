#include "Services.h"
#include <utility>

Service::Service() : _name(), _pro(false), _basePrice(0), _duration(Duration(0, 0)) {}

Service::Service(std::string name, bool pro, float basePrice, const Duration &duration) :
        _name(std::move(name)), _pro(pro), _basePrice(basePrice), _duration(duration){}

std::string Service::getName() const{
    return _name;
}

bool Service::getPro() const{
    return _pro;
}

float Service::getBasePrice() const{
    return _basePrice;
}

Duration Service::getDuration() const{
    return _duration;
}

float Service::calculatePrice() {
    return _basePrice;
}

Painting::Painting(std::string name, bool pro, float basePrice, const Duration &duration, unsigned int nrOfRooms) :
                    Service(std::move(name), pro, basePrice, duration), _roomNumber(nrOfRooms) {}

void Painting::setRoomNumber(unsigned int number) {
    _roomNumber = number;
}

float Painting::calculatePrice() {
    float price = getBasePrice();
    float sum = 0;
    for(unsigned int n = 1; n < _roomNumber + 1; n++){
        if(n > 1 && price >= MinimumPricePerRoom) {
            price = float(ROOM_DISCOUNT) * price;
        }
        sum += price;
    }
    return sum;
}


Intervention::Intervention(const Date& appointment, Service* type, bool forcePro, unsigned int nrOfRooms) :
        _startingTime(appointment), _forcePro(forcePro), _state(Scheduled), _cost(), _paid(false){

    auto painting = dynamic_cast<Painting*>(type);
    if (painting) {
        _type = new Painting(type->getName(), type->getPro(), type->getBasePrice(), type->getDuration(),nrOfRooms);
    }
    else{
        _type = type;
    }
}

Intervention::~Intervention() {
    delete _type;
}

const Service* Intervention::getService()const{
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

void Intervention::setProcessState(processState state) {
    _state = state;
}

bool Intervention::getForcePro() const {
    return _forcePro;
}

float Intervention::getCost() const {
    return _cost;
}

bool Intervention::getPaid() const {
    return _paid;
}

std::string Intervention::getCollabId() const {
    return _collabId;
}

std::string Intervention::getClientId() const {
    return _clientId;
}

void Intervention::setCollabId(const std::string& collabId) {
    _collabId = collabId;
}

void Intervention::setClientId(const std::string& clientId) {
    _clientId = clientId;
}

Date Intervention::getStartingTime() const {
    return _startingTime;
}

bool Intervention::conflictsWith(Date start, Duration duration) const{

    Date otherStart = start;
    Date otherEnd = start + duration;

    return (getEndTime() > otherStart && getStartingTime() < otherStart) ||
    (getStartingTime() < otherEnd && getEndTime() > otherEnd) ||
    getStartingTime() == otherStart || getEndTime() == otherStart || getStartingTime() == otherEnd || getEndTime() == otherEnd;
}

void Intervention::calculateCost() {
    auto painting = dynamic_cast<Painting*>(_type);
    if(painting) {
        _cost = float(1 + HouseMasterTax) * painting->calculatePrice();
    }
    else {
        _cost = float(1 + HouseMasterTax) * _type->calculatePrice();
    }
}

Date Intervention::getEndTime() const {
    return _startingTime + _type->getDuration();
}

void Intervention::pay() {
    _paid = true;
}

bool Intervention::isActive() const {
    return _state != Complete && _state != Canceled;
}
