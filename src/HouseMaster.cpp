
#include "HouseMaster.h"

#include <utility>

HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients) {

    for (std::string line; std::getline(collaborators, line); ) {
        auto collaborator = new Individual(line);
        this->_collaborators.push_back(collaborator);
    }

    for (std::string line; std::getline(clients, line); ) {
        auto client = new Individual(line);
        this->_clients.push_back(client);
    }

}

const std::vector<Individual *> &HouseMaster::getCollaborators() const {
    return _collaborators;
}

const std::vector<Individual *> &HouseMaster::getClients() const {
    return _clients;
}

const std::vector<servicesType *> &HouseMaster::getAvailableServices() const {
    return _availableServices;
}

const std::vector<Intervention *> &HouseMaster::getInterventions() const {
    return _interventions;
}

void HouseMaster::addAvailableService(std::string service, bool pro) {
    auto availableService = new servicesType;
    availableService->type = std::move(service);
    availableService->pro = pro;
    _availableServices.push_back(availableService);
}

void HouseMaster::removeAvailableService(std::string service) {

}
