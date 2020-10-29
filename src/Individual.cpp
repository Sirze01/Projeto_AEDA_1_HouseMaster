#include "Individual.h"

#include <utility>
#include <algorithm>


Individual::Individual(std::string name) : _name(std::move(name)) {}

std::string Individual::getName() {
    return _name;
}

unsigned int Individual::getId() const {
    return _id;
}

bool Individual::operator==(const Individual& ind2) {
    if(_name == ind2._name || _id == ind2._id)
        return true;
    return false;
}


// Collaborator associated methods
unsigned Collaborator::_idSeq = 0;

std::vector<Service*> Collaborator::getServices() {
    return _services;
}

int Collaborator::getScore() {
    return _score;
}

bool Collaborator::isAvailable(date* appointment) {
    for(const auto &occupied : _avaiability){
        if(*appointment == *occupied){
            return false;
        }
    }
    return true;
}

Collaborator::Collaborator(const std::vector<Service *>& functions, const std::string& name) : Individual(name),
    _services(functions), _score(newHere) {}

bool Collaborator::canPreform(Service* service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](Service* service1) {
        Service s1 = *service;
        Service s2 = *service1;
        return s1.name == s2.name;
    });
    return found != _services.end();
}

void Collaborator::addAppointment(date *date) {
    _avaiability.push_back(date);
}

// Client associated methods
unsigned Client::_idSeq = 0;

Client::Client(unsigned int nif, const std::string &name) : Individual(name), _nif(nif), _requestedInterventions() {}

unsigned int Client::getNif()
{
    return(_nif);
}

int Client::requestIntervention(date appointment, Service type, bool forcePro)
{
    auto intervention = new Intervention(appointment, type, forcePro);
    _requestedInterventions.push_back(intervention);
    return 0;
}

const std::vector<Intervention *> &Client::getRequestedInterventions() const {
    return _requestedInterventions;
}

