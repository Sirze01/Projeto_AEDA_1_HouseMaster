#include "Individual.h"

#include <utility>
#include <algorithm>
#include <sstream>

unsigned int Collaborator::_idSeqCol = 0;
unsigned int Client::_idSeqClt = 0;

Individual::Individual():_displayName(), _id() {}

Individual::Individual(std::string  name) : _displayName(std::move(name)), _id(0) {
}

std::string Individual::getName() {
    return _displayName;
}




// Collaborator associated methods

Collaborator::Collaborator(std::vector<std::string> functions, const std::string &name, bool pro) : Individual(name),
                                                                                                    _services(std::move(functions)),
                                                                                                    _score(newHere), _pro(pro) {
    _id = _idSeqCol++;
}

std::vector<std::string> Collaborator::getServices() {
    return _services;
}

bool Collaborator::isPro() const {
    return _pro;
}

bool Collaborator::canPreform(const std::string& service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](const std::string& service1) {
        return service == service1;
    });
    return found != _services.end();
}

bool Collaborator::hasQualificationToPreform(Intervention *intervention) const {
    return !(intervention->getService()->pro || intervention->getForcePro()) || isPro();
}

int Collaborator::getScore() {
    return _score;
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

std::string Collaborator::getId() const {
    std::stringstream outStr;
    outStr << "collab" << _id;
    return outStr.str();
}
bool Collaborator::operator==(const Collaborator& ind2) const {
    return _id == ind2._id;
}

bool Collaborator::operator<(const Collaborator &col2) const{
    return _score < col2._score;
}



// Client associated methods

Client::Client(unsigned int nif, const std::string &name, bool premium)
        : Individual(name), _nif(nif), _premium(premium) {
    _id = _idSeqClt++;
}

unsigned int Client::getNif() {
    return (_nif);
}

bool Client::isPremium() const {
    return _premium;
}

std::string Client::getId() const {
    std::stringstream outStr;
    outStr << "client" << _id;
    return outStr.str();
}



bool Client::operator==(const Client& ind2) const {
    return _id == ind2._id;
}
