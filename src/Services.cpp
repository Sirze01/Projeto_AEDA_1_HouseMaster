#include"Services.h"

// Despite the static variables being already 0 initialized
unsigned Intervention::_idSeq = 0;

Intervention::Intervention(date appointment, Service type, bool forcePro): _appointment(appointment), _type(type),
    _forcePro(forcePro), _id(++_idSeq), _state(Scheduled) {

}

Service Intervention::getService() {
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

float Intervention::getPrice() const {
    return _price;
}
