#include "Services.h"
#include <utility>

Service::Service(std::string name, bool pro, float basePrice, const date &duration) :
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

date Service::getDuration() const{
    return _duration;
}

float Service::calculatePrice() {
    return _basePrice;
}

Painting::Painting(std::string name, bool pro, float basePrice, const date &duration) : Service(std::move(name), pro, basePrice, duration), _roomNumber(0) {}

void Painting::setRoomNumber(int number) {
    _roomNumber = number;
}

float Painting::calculatePrice() {
    float price = getBasePrice();
    float sum = 0;
    for(int n = 1; n < _roomNumber + 1; n++){
        if(n == 1)
            sum += price;
        else if(price >= MinimumPricePerRoom) {
            price = float(ROOM_DISCOUNT) * price;
            sum += price;
        }
        else
            sum += price;
    }
    return sum;
}


Intervention::Intervention(const date& appointment, Service  type, bool forcePro) :
        _startingTime(appointment), _type(std::move(type)), _forcePro(forcePro), _state(Scheduled), _cost(){}

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

const date* Intervention::getStartingTime() const {
    return &_startingTime;
}

//Tentar usar referencia para outra intervention
bool Intervention::conflictsWith(date start, date duration) {

    date interventionStart = _startingTime;

    date otherStart = start;
    date otherEnd = start + duration;

    return (interventionStart > otherStart && interventionStart < otherEnd) || (getEndTime() > otherStart && interventionStart < otherStart);
}

void Intervention::calculateCost() {
    _cost = float(1 + HouseMasterTax) * _type.calculatePrice();
}

date Intervention::getEndTime() const {
    return _startingTime + _type.getDuration();
}

bool Intervention::isActive() const {
    return _state == InProgress || _state == Scheduled;
}
