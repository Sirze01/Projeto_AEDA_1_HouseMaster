
#include "HouseMaster.h"

#include <utility>

HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients) {

    for (std::string line; std::getline(collaborators, line); ) {
        std::istringstream lss(line);
        std::string name{}, service{};
        std::vector<servicesType *> services{};
        std::getline(lss, name, ',');
        while (std::getline(lss, service, ',')) {
            auto *s = new servicesType;
            s->type = service;
            s->pro = false;
            services.push_back(s);
        }
        auto collaborator = new Collaborator(services, name);
        this->_collaborators.push_back(collaborator);
    }

    for (std::string line; std::getline(clients, line); ) {
        std::stringstream lss(line);
        std::string name;
        unsigned int nif;
        std::getline(lss, name, ',');
        lss >> nif;
        auto client = new Client(nif, name);
        this->_clients.push_back(client);
    }

}

const std::vector<Collaborator *> &HouseMaster::getCollaborators() const {
    return _collaborators;
}

const std::vector<Client *> &HouseMaster::getClients() const {
    return _clients;
}

const std::vector<servicesType *> &HouseMaster::getAvailableServices() const {
    return _availableServices;
}

const std::vector<Intervention *> &HouseMaster::getInterventions() const {
    return _interventions;
}

void HouseMaster::addAvailableService(servicesType *service) {
    _availableServices.push_back(service);
}

void HouseMaster::removeAvailableService(servicesType *service) {
    auto it = std::find(_availableServices.begin(), _availableServices.end(), service);
    if (it != _availableServices.end()) {
        delete *it;
        it = _availableServices.erase(it);
    } else {
        // throw error
    }
}

