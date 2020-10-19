#include "individuals.h"

individual::individual(std::string name) : _name(name) {}

std::string individual::getName() {
    return _name;
}

unsigned int individual::getId() {
    return _id;
}

bool individual::operator==(const individual& right) {
    if(_name == right._name || _id == right._id)
        return true;
    return false;
}
