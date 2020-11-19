#include "Services.h"
#include <utility>

Service::Service() : _name(), _pro(false), _basePrice(0), _duration(duration(0,0)) {}

Service::Service(std::string name, bool pro, float basePrice, const duration &duration) :
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

duration Service::getDuration() const{
    return _duration;
}

float Service::calculatePrice() {
    return _basePrice;
}

Painting::Painting(std::string name, bool pro, float basePrice, const duration &duration) : Service(std::move(name), pro, basePrice, duration), _roomNumber(0) {}

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


Intervention::Intervention(const date& appointment, const Service& type, bool forcePro, unsigned int nrOfRooms) :
        _startingTime(appointment), _type(type), _forcePro(forcePro), _state(Scheduled), _cost(), _paid(false){
    if (_type.getName() == "Painting"){
        if (nrOfRooms == 0) {}
        else {
            auto *ptr = dynamic_cast<Painting *>(&_type);
            ptr->setRoomNumber(nrOfRooms);
        }
    }
}

const Service* Intervention::getService()const{
    return &_type;
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

date Intervention::getStartingTime() const {
    return _startingTime;
}

bool Intervention::conflictsWith(date start, duration duration) const{

    date otherStart = start;
    date otherEnd = start + duration;

    return (getEndTime() > otherStart && getStartingTime() < otherStart) ||
    (getStartingTime() < otherEnd && getEndTime() > otherEnd) ||
    getStartingTime() == otherStart || getEndTime() == otherStart || getStartingTime() == otherEnd || getEndTime() == otherEnd;
}

void Intervention::calculateCost() {
    _cost = float(1 + HouseMasterTax) * _type.calculatePrice();
}

date Intervention::getEndTime() const {
    return _startingTime + _type.getDuration();
}

void Intervention::pay() {
    _paid = true;
}

bool Intervention::isActive() const {
    return _state != Complete && _state != Canceled;
}
