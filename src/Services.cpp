#include"Services.h"
#include <utility>

// Despite the static variables being already 0 initialized
//unsigned Intervention::_idSeq = 0;

/*
Intervention::Intervention(date appointment, Service type, bool forcePro) : _startingTime(appointment),_type(std::move(type)),
                                                                            _forcePro(forcePro), _id(++_idSeq),
                                                                            _state(Scheduled), _price() {}
*/

Intervention::Intervention(date appointment, Service type, bool forcePro) : _startingTime(appointment),_type(std::move(type)),
                                                                            _forcePro(forcePro),_state(Scheduled), _price() {}

Service* Intervention::getService() {
    return &_type;
}

processState Intervention::getProcessState() {
    return _state;
}

bool Intervention::getForcePro() const {
    return _forcePro;
}

float Intervention::getPrice() const {
    return _price;
}

std::string Intervention::getCollabId() const {
    return _collabId;
}

std::string Intervention::getClientId() const {
    return _clientId;
}

void Intervention::setCollabId(unsigned int collabId) {
    _collabId = collabId;
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

date Intervention::getEndTime() const {
    return _startingTime + _type.duration;
}

Service::Service(const std::string &name, bool pro, float basePrice, const date &duration) : name(name), pro(pro), basePrice(basePrice), duration(duration){

}
