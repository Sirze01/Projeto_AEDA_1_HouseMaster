#include "Individual.h"

#include <utility>
#include <algorithm>


Individual::Individual(std::string name) : _displayName(std::move(name)), _id() {
}

std::string Individual::getName() {
    return _displayName;
}

unsigned int Individual::getId() const {
    return _id;
}

bool Individual::operator==(const Individual& ind2) const {
    return _uniqueName == ind2._uniqueName;
}

const std::vector<Intervention *> &Individual::getAssociatedInterventions() const {
    return _associatedInterventions;
}

std::string Individual::getInfo() const {
    return std::string();
}

std::string Individual::getUniqueName() const {
    return _uniqueName;
}


// Collaborator associated methods
unsigned Collaborator::_idSeq = 0;

std::vector<Service *> Collaborator::getServices() {
    return _services;
}

int Collaborator::getScore() {
    return _score;
}


Collaborator::Collaborator(std::vector<Service *> functions, const std::string &name, bool pro) : Individual(name),
                                                                                               _services(std::move(functions)),
                                                                                               _score(newHere), _pro(pro) {
    _id = _idSeq++;
    std::stringstream ss;
    ss << "collab" << _id;
    _uniqueName = ss.str();
}


bool Collaborator::canPreform(Service *service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](Service *service1) {
        Service s1 = *service;
        Service s2 = *service1;
        return s1.name == s2.name;
    });
    return found != _services.end();
}


bool Collaborator::isAvailable(date start, date duration) {
    for (const auto &intervention : getAssociatedInterventions()) {
        if (intervention->conflictsWith(start, duration)) {
            return false;
        }
    }
    return true;
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
        _score = static_cast<Classification>(round(average));
    }
}

bool Collaborator::hasQualificationToPreform(Intervention *intervention) const {
    return !(intervention->getService()->pro || intervention->getForcePro()) || isPro();
}

bool Collaborator::isPro() const {
    return _pro;
}

bool Collaborator::canDo(Intervention *intervention) {
    Service *service = intervention->getService();
    date start = *intervention->getStartingTime();
    date duration = intervention->getService()->duration;
    return isAvailable(start, duration) && canPreform(service) && hasQualificationToPreform(intervention);
}

std::string Collaborator::getInfo() const {
    std::stringstream ss{};
    ss << "Name: " << _displayName << " Score: " << _score;
    return ss.str();
}




// Client associated methods
unsigned Client::_idSeq = 0;

Client::Client(unsigned int nif, const std::string &name, bool premium)
        : Individual(name), _nif(nif), _requestedInterventions(), _premium(premium) {
    _id = _idSeq++;
    std::stringstream ss;
    ss << "client" << _id;
    _uniqueName = ss.str();
}

unsigned int Client::getNif() {
    return (_nif);
}

int Client::requestIntervention(date appointment, Service type) {
    auto intervention = new Intervention(appointment, std::move(type), _premium);
    _requestedInterventions.push_back(intervention);
    return 0;
}

const std::vector<Intervention *> &Client::getRequestedInterventions() const {
    return _requestedInterventions;
}

bool Client::isPremium() const {
    return _premium;
}

std::string Client::getInfo() const {
    std::stringstream ss{};
    std::string pro = _premium ? "yes" : "no";
    ss << "Name: " << _displayName << " NIF: " << _nif << " Pro: " << pro;
    return ss.str();
}

