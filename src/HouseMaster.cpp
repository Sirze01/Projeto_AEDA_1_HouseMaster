#include "HouseMaster.h"

HouseMaster::InexistentService::InexistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

HouseMaster::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg): std::logic_error(error_msg) {}

HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services) {

    // read services.txt
    for (std::string line; std::getline(services, line);) {

        std::stringstream lss(line);
        // name
        std::string name;
        std::getline(lss, name, ',');
        // pro
        std::string proStr;
        std::getline(lss, proStr, ',');
        bool pro = proStr == "yes";
        // price
        std::string priceStr;
        std::getline(lss, priceStr, ',');
        float price = std::stof(priceStr);
        // duration
        std::string durationStr;
        std::getline(lss, durationStr, ',');
        date duration; duration.readDuration(durationStr);

        auto service = new Service(name, pro, price, duration);
        _availableServices.push_back(service);

    }

    // read collaborators.txt
    for (std::string line; std::getline(collaborators, line);) {
        std::stringstream lss(line);
        // name
        std::string name;
        std::getline(lss, name, ',');
        // pro
        std::string proStr;
        std::getline(lss, proStr, ',');
        // services
        std::string serviceName{};
        std::vector<Service* > collabServices{};
        while (std::getline(lss, serviceName, ',')) {
            auto *service = findServiceByName(serviceName);
            collabServices.push_back(service);
        }
        auto collaborator = new Collaborator(collabServices, name, proStr=="yes");
        _collaborators.push_back(collaborator);
    }

    // read clients.txt
    for (std::string line; std::getline(clients, line);) {
        std::stringstream lss(line);
        std::string name;
        std::string nifStr{};
        std::string premiumStr{};
        std::getline(lss, name, ',');
        std::getline(lss, nifStr, ',');
        std::getline(lss, premiumStr, ',');
        auto client = new Client(std::stoul(nifStr), name, premiumStr == "yes");
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

Service *HouseMaster::findServiceByName(const std::string &name) {
    auto found = std::find_if(_availableServices.begin(), _availableServices.end(), [&name](Service* s1) {
        return s1->name == name;
    });
    // exception!!
    return *found;
}

Client *HouseMaster::findClientByUniqueName(const std::string &name) {
    auto found = std::find_if(_clients.begin(), _clients.end(), [&name](Client* c1) {
        return c1->getUniqueName() == name;
    });
    // exception!!
    return *found;
}

Collaborator *HouseMaster::findCollabByUniqueName(const std::string &name) {
    auto found = std::find_if(_collaborators.begin(), _collaborators.end(), [&name](Collaborator* c1) {
        return c1->getUniqueName() == name;
    });
    // exception!!
    return *found;
}


void HouseMaster::assignColaborator(Intervention * intervention) {

    sortCollaboratorsByScore();

    auto found = std::find_if(_collaborators.begin(), _collaborators.end(),
                              [&intervention](Collaborator* collaborator) -> bool{
        return collaborator->canDo(intervention);
    });

    if(found != _collaborators.end()) {
        intervention->setCollabId(_collaborators.at(std::distance(_collaborators.begin(), found))->getId());
    }
    else
        throw UnavailableAppointment("No collaborators available to the desired date!");
}


