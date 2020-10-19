#include"Services.h"

void Intervention::idSeqSetter() {
    _idSeq = 0;
}

Intervention::Intervention(date appointment, servicesType type, bool forcePro): _appointment(appointment),
                            _type(type), _forcePro(forcePro), _id(++_idSeq){}