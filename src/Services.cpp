#include"Services.h"

unsigned Intervention::_idSeq = 0;

Intervention::Intervention(date appointment, servicesType type, bool forcePro): _appointment(appointment), _type(type),
    _forcePro(forcePro), _id(++_idSeq){}

servicesType Intervention::getType() {
    return _type;
}

processState Intervention::getProcessState() {
    return _state;
}

float Intervention::getPrice() {
    return _price;
}