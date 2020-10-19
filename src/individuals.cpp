#include "individuals.h"

individual::individual(std::string name) : _name(name) {}

std::string individual::getName() {
    return _name;
}

unsigned int individual::getId() {
    return _id;
}

std::vector<interventions>::iterator individual::findIndividual(std::string name, unsigned int id) {

}
