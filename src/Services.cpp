#include"Services.h"

unsigned Intervention::_idSeq = 0;

Intervention::Intervention(date appointment, servicesType type, bool forcePro, float price): _appointment(appointment), _type(type),
    _forcePro(forcePro), _id(++_idSeq), _price(price), _state(Scheduled) {

}

servicesType Intervention::getType() {
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

float Intervention::getPrice() const {
    return _price;
}
