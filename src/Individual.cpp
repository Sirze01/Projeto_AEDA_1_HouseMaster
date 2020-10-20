#include "Individual.h"

#include <utility>

unsigned Individual::_idSeq = 0;

Individual::Individual(std::string name) : _name(std::move(name)), _id(++_idSeq) {
}

std::string Individual::getName() {
    return _name;
}

unsigned int Individual::getId() const {
    return _id;
}

bool Individual::operator==(const Individual& right) {
    if(_name == right._name || _id == right._id)
        return true;
    return false;
}
