#include "HouseMaster.h"

HouseMaster::InexistentService::InexistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

HouseMaster::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg): std::logic_error(error_msg) {}

HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients) {
    for (std::string line; std::getline(collaborators, line);) {
        std::istringstream lss(line);
        std::string name{}, service{};
        std::vector<Service *> services{};
        std::getline(lss, name, ',');
        while (std::getline(lss, service, ',')) {
            auto *s = new Service;
            s->name = service;
            s->pro = false;
            services.push_back(s);
            _availableServices.push_back(s);
        }
        auto collaborator = new Collaborator(services, name);
        this->_collaborators.push_back(collaborator);
    }

    for (std::string line; std::getline(clients, line);) {
        std::stringstream lss(line);
        std::string name;
        unsigned int nif{};
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

const std::vector<Service *> &HouseMaster::getAvailableServices() const {
    return _availableServices;
}

const std::set<std::pair<Client *, Intervention *> > &HouseMaster::getInterventions() const {
    return _interventions;
}

void HouseMaster::addAvailableService(Service *service) {
    _availableServices.push_back(service);
}

void HouseMaster::removeAvailableService(Service *service) {
    try {
        auto it = std::find(_availableServices.begin(), _availableServices.end(), service);
        if (it != _availableServices.end()) {
            delete *it;
        } else {
            throw InexistentService("There's no such service!");
        }
    }
    catch (InexistentService &error) {
        std::cerr << error.what();
    }
}

void HouseMaster::updateInterventions() {
    for (const auto &client : _clients) {
        for (const auto &intervention : client->getRequestedInterventions()) {
            Intervention *inter = intervention;
            auto registry = std::make_pair(client, inter);
            _interventions.insert(registry);
        }
    }
}

void HouseMaster::sortCollaboratorsByScore() {
    std::sort(_collaborators.begin(), _collaborators.end(), [](Collaborator* collaborator1, Collaborator* collaborator2) {
        return collaborator1->getScore() > collaborator2->getScore();
    });
}

void HouseMaster::assignColaborator(Intervention * intervention) {
   auto it = std::find(_collaborators.begin(), _collaborators.end(),
                        [&intervention](Collaborator* &collaborator){
       return ( collaborator -> canPreform(intervention->getService()) &&
                collaborator ->isAvailable(intervention->getStartingTime()));
    });

    if(it != _collaborators.end()) {
        _collaborators.at(std::distance(_collaborators.begin(), it))->addAppointment(intervention->getAppointment());
    }
    else
        throw UnavailableAppointment("No collaborators available to the desired date!");
}


