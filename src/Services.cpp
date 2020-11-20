#include "Services.h"

/**
 * @brief service's constructor
 */
Service::Service() : _name(), _pro(false), _basePrice(0), _duration(Duration(0, 0)) {}

/**
 * @brief service's constructor
 * @param name the name
 * @param pro requires professional
 * @param basePrice the base price
 * @param duration the duration
 */
Service::Service(std::string name, bool pro, float basePrice, Duration duration) :
        _name(std::move(name)), _pro(pro), _basePrice(basePrice), _duration(std::move(duration)) {

}

/**
 * @brief getter
 * @return name
 */
std::string Service::getName() const {
    return _name;
}

/**
 * @brief getter
 * @return true if requires pro false otherwise
 */
bool Service::getPro() const {
    return _pro;
}

/**
 * @brief getter
 * @return base price
 */
float Service::getBasePrice() const {
    return _basePrice;
}

/**
 * @brief getter
 * @return duration
 */
Duration Service::getDuration() const {
    return _duration;
}

/**
 * @brief getter
 * @return price
 */
float Service::getPrice() {
    return _basePrice;
}

/**
 * @brief painting service constructor
 * @param name the name
 * @param pro requires professional help
 * @param basePrice base price
 * @param duration duration
 * @param nrOfRooms number of rooms
 */
Painting::Painting(std::string name, bool pro, float basePrice, const Duration &duration, unsigned int nrOfRooms) :
        Service(std::move(name), pro, basePrice, duration), _roomNumber(nrOfRooms) {

}

/**
 * @brief setter
 * @param number number of rooms
 */
void Painting::setRoomNumber(unsigned int number) {
    _roomNumber = number;
}

/**
 * @brief getter
 * @return price
 */
float Painting::getPrice() {
    float price = getBasePrice();
    float sum = 0;
    for (unsigned int n = 1; n < _roomNumber + 1; n++) {
        if (n > 1 && price >= MinimumPricePerRoom) {
            price = float(ROOM_DISCOUNT) * price;
        }
        sum += price;
    }
    return sum;
}

/**
 * @brief intervention's constructor
 * @param appointment starting
 * @param type name
 * @param forcePro
 * @param nrOfRooms number of rooms to paint in case of painting service
 */
Intervention::Intervention(Date appointment, Service *type, bool forcePro, unsigned int nrOfRooms) :
        _startingTime(std::move(appointment)), _forcePro(forcePro), _state(Scheduled), _cost(), _paid(false) {

    auto painting = dynamic_cast<Painting *>(type);
    if (painting) {
        _type = new Painting(type->getName(), type->getPro(), type->getBasePrice(), type->getDuration(), nrOfRooms);
    } else {
        _type = type;
    }
}

/**
 * @brief intervention destructor
 */
Intervention::~Intervention() {
    delete _type;
}

/**
 * @brief getter
 * @return service
 */
const Service *Intervention::getService() const {
    return _type;
}

/**
 * @brief getter
 * @return state
 */
processState Intervention::getProcessState() {
    return _state;
}

/**
 * @brief setter
 * @param state state
 */
void Intervention::setProcessState(processState state) {
    _state = state;
}

/**
 * @brief getter
 * @return forcepro
 */
bool Intervention::getForcePro() const {
    return _forcePro;
}

/**
 * @brief getter
 * @return cost
 */
float Intervention::getCost() const {
    return _cost;
}

/**
 * @brief getter
 * @return is paid
 */
bool Intervention::getPaid() const {
    return _paid;
}

/**
 * @brief getter
 * @return collab's id
 */
std::string Intervention::getCollabId() const {
    return _collabId;
}

/**
 * @brief getter
 * @return client's id
 */
std::string Intervention::getClientId() const {
    return _clientId;
}

/**
 * @brief setter
 * @param collabId collab's id
 */
void Intervention::setCollabId(const std::string &collabId) {
    _collabId = collabId;
}

/**
 * @brief setter
 * @param clientId client's id
 */
void Intervention::setClientId(const std::string &clientId) {
    _clientId = clientId;
}

/**
 * @brief getter
 * @return starting time
 */
Date Intervention::getStartingTime() const {
    return _startingTime;
}

/**
 * @brief checks if the intervention conflicts with another intervention
 * @param start the starting of the other intervention
 * @param duration the duration of the other intervention
 * @return true if they conflict false if not
 */
bool Intervention::conflictsWith(const Date& start, const Duration& duration) const {

    const Date& otherStart = start;
    Date otherEnd = start + duration;

    return (getEndTime() > otherStart && getStartingTime() < otherStart) ||
           (getStartingTime() < otherEnd && getEndTime() > otherEnd) ||
           getStartingTime() == otherStart || getEndTime() == otherStart || getStartingTime() == otherEnd ||
           getEndTime() == otherEnd;
}

/**
 * @brief calculates the cost
 */
void Intervention::calculateCost() {
    auto painting = dynamic_cast<Painting *>(_type);
    if (painting) {
        _cost = float(1 + HouseMasterTax) * painting->getPrice();
    } else {
        _cost = float(1 + HouseMasterTax) * _type->getPrice();
    }
}

/**
 * @brief getter
 * @return ending
 */
Date Intervention::getEndTime() const {
    return _startingTime + _type->getDuration();
}

/**
 * @brief pay an interverntion
 */
void Intervention::pay() {
    _paid = true;
}

/**
 * @brief getter
 * @return true if intervention is active false otherwise
 */
bool Intervention::isActive() const {
    return _state != Complete && _state != Canceled;
}
