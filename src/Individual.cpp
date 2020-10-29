#include "Individual.h"

#include <utility>
#include <algorithm>


Individual::Individual(std::string name) : _name(std::move(name)), _id() {
}

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

const std::vector<Intervention *> &Individual::getAssociatedInterventions() const {
    return _associatedInterventions;
}


// Collaborator associated methods
unsigned Collaborator::_idSeq = 0;

std::vector<Service *> Collaborator::getServices() {
    return _services;
}

int Collaborator::getScore() {
    return _score;
}


Collaborator::Collaborator(std::vector<Service *> functions, const std::string &name) : Individual(name),
                                                                                               _services(std::move(functions)),
                                                                                               _score(newHere) {}


bool Collaborator::canPreform(Service *service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](Service *service1) {
        Service s1 = *service;
        Service s2 = *service1;
        return s1.name == s2.name;
    });
    return found != _services.end();
}

void Collaborator::addAppointment(date *date) {
    _avaiability.push_back(date);
}

bool Collaborator::isAvailable(date start, date duration) {
    for (const auto &intervention : getAssociatedInterventions()) {
        date interventionStart = intervention->getStartingTime();
        date interventionEnd = intervention->getStartingTime() + intervention->getService()->duration;
        // TODO - faltava operador < e >. mt trst.

    }
    return false;
}

void Collaborator::addClassification(Classification classification) {
    _classifications.push_back(classification);
}

void Collaborator::updateScore() {
    double sum{}, average{};
    if(_classifications.empty()) _score = newHere;
    else {
        std::for_each(_classifications.begin(), _classifications.end(), [&] (int n) {
            sum += n;
        });
        average = sum / static_cast<double>(_classifications.size());
        if (average > savior) _score = savior;
        else if (average < unreliable) _score = unreliable;
        else _score = static_cast<Classification>(round(average));
    }
}


// Client associated methods
unsigned Client::_idSeq = 0;

Client::Client(unsigned int nif, const std::string &name) : Individual(name), _nif(nif), _requestedInterventions() {}

unsigned int Client::getNif() {
    return (_nif);
}

int Client::requestIntervention(date appointment, Service type, bool forcePro) {
    auto intervention = new Intervention(appointment, std::move(type), forcePro);
    _requestedInterventions.push_back(intervention);
    return 0;
}

const std::vector<Intervention *> &Client::getRequestedInterventions() const {
    return _requestedInterventions;
}

