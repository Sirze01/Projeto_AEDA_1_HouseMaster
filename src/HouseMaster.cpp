#include "HouseMaster.h"

bool colComparer(std::pair<std::string, Collaborator *> &a, std::pair<std::string, Collaborator *> &b) {
    return a.second->getScore() > b.second->getScore();
}

HouseMaster::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg) : std::logic_error(
        error_msg) {}

HouseMaster::InexistentService::InexistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

HouseMaster::ExistentService::ExistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

HouseMaster::InexistentCollab::InexistentCollab(const std::string &error_msg) : std::out_of_range(error_msg) {}

HouseMaster::InexistentClient::InexistentClient(const std::string &error_msg) : std::out_of_range(error_msg){}

HouseMaster::ExistentClient::ExistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

std::vector<Intervention*> Individual::getAssociatedInterventions(HouseMaster &hm, const std::string &clientId) {
    return hm.getAssociatedInterventions(clientId);
}

void Client::requestIntervention(HouseMaster& hm, const date &_date, const std::string &type, const std::string &clientId,
                                 bool forcePro) {
    hm.addIntervention(_date, type, forcePro, clientId);
}

void Client::cancelIntervention(HouseMaster &hm, Intervention * intervention) {
    hm.changeinterventionState(intervention, Canceled);
}

void Client::classifyCollaborator(HouseMaster &hm, const std::string &collabId, Classification classification) {
    hm.getCollaborators()[collabId]->addClassification(classification);
}

bool Collaborator::isAvailable(HouseMaster &hm, const std::string &collabId, date start, date duration) {
    for (const auto &intervention : hm.getAssociatedInterventions(collabId)) {
        if (intervention->conflictsWith(start, duration)) {
            return false;
        }
    }
    return true;
}

bool Collaborator::canDo(HouseMaster& hm, const std::string &collabId, Intervention *intervention) {
    const Service *service = intervention->getService();
    date start = *intervention->getStartingTime();
    date duration = service->duration;
    return isAvailable(hm, collabId, start, duration) && canPreform(service->name) && hasQualificationToPreform(intervention);
}

void Collaborator::markInterventionAsInProgress(HouseMaster &hm, Intervention *a) {
    hm.changeinterventionState(a, InProgress);
}

void Collaborator::markInterventionAsComplete(HouseMaster &hm, Intervention *a) {
    hm.changeinterventionState(a, Complete);
}

HouseMaster::HouseMaster() : _availableServices(), _clients(), _collaborators(), _interventions() {}

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
        date duration;
        duration.readDuration(durationStr);

        addAvailableService(name, pro, price, duration);
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
        std::vector<std::string> collabServices;
        while (std::getline(lss, serviceName, ',')) {
            collabServices.push_back(serviceName);
        }

        addCollaborator(collabServices, name, proStr == "yes");
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

        addClient(std::stoul(nifStr), name, premiumStr == "yes");
    }
}

std::unordered_map<std::string, Collaborator *> &HouseMaster::getCollaborators() {
    return _collaborators;
}


std::vector<Intervention *> &HouseMaster::getInterventions() {
    return _interventions;
}

void HouseMaster::addAvailableService(const std::string &name, bool pro, float basePrice, date duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        auto service = new Service(name, pro, basePrice, duration);
        _availableServices.insert({name, service});
    }
}

void HouseMaster::removeAvailableService(const std::string &serviceName) {

    auto it = _availableServices.find(serviceName);
    if (it != _availableServices.end()) {
        delete it->second;
        _availableServices.erase(it);
    } else {
        throw InexistentService("There's no such service!");
    }
}

std::unordered_map<std::string, Service *> &HouseMaster::getAvailableServices() {
    return _availableServices;
}

void HouseMaster::addCollaborator(const std::vector<std::string> &functions, const std::string &name, bool pro) {
    auto collab = new Collaborator(functions, name, pro);
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}

/*
void HouseMaster::addCollaborator(const std::string &username, Collaborator *collaborator) {
    _collaborators.insert(std::pair<std::string, Collaborator *>(collaborator->getId(), collaborator));
    _usernameMap.insert({username, collaborator->getId()});

}
 */

void HouseMaster::removeCollaborator(const std::string &id) {
    auto it = _collaborators.find(id);
    auto UsernameIt = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                                   [&id](const std::pair<std::string, std::string> &mapped) {
                                       return mapped.second == id;
                                   });
    if (it != _collaborators.end()) {
        if (UsernameIt != _usernameMap.end()) {
            _collaborators.erase(it);
            _usernameMap.erase(UsernameIt);
        } else {
            //throws except
        }

    } else {
        throw InexistentCollab("There's no such collab!");
    }
}

/*
void HouseMaster::deleteCollaborator(const std::string &id) {
    auto it = _collaborators.find(id);
    auto UsernameIt = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                                   [&id](const std::pair<std::string, std::string> &mapped) {
                                       return mapped.second == id;
                                   });
    if (it != _collaborators.end()) {
        if (UsernameIt != _usernameMap.end()) {
            delete it->second;
            _collaborators.erase(it);
            _usernameMap.erase(UsernameIt);
        } else {
            //throws except
        }

    } else {
        //throws except
    }
}
*/

void HouseMaster::addClient(unsigned int nif, const std::string &name, bool premium) {
    auto it = std::find_if(_clients.begin(), _clients.end(), [&nif](const std::pair<std::string, Client*> &pair){
                               if(pair.second->getNif() == nif)
                                   return true;
                               else
                                   return false;
                           });
    if(it == _clients.end()) {
        auto client = new Client(nif, name, premium);
        _clients.insert({client->getId(), client});
        _usernameMap.insert({client->getId(), client->getId()});
    }
    else{
        throw HouseMaster::ExistentClient("Client already registred!");
    }
}

void HouseMaster::removeClient(const std::string &clientId) {

    auto ClientIt = _clients.find(clientId);
    auto UsernameIt = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                                   [&clientId](const std::pair<std::string, std::string> &mapped) {
                                       return mapped.second == clientId;
                                   });
    if (ClientIt != _clients.end()) {
        if (UsernameIt != _usernameMap.end()) {
            delete ClientIt->second;
            _clients.erase(ClientIt);
            _usernameMap.erase(UsernameIt);
        } else {
            //throws except
        }

    } else {
        throw InexistentClient("Theres no such client!");
    }

}

std::unordered_map<std::string, Client *> &HouseMaster::getClients() {
    return _clients;
}

void HouseMaster::addIntervention(const date &appointment, const std::string &type, bool forcePro, const std::string &clientId) {
    auto it = _availableServices.find(type);
    if (it != _availableServices.end()) {
        auto newIntervention = new Intervention(appointment, *_availableServices[type], forcePro);
        newIntervention->setClientId(clientId);
        _interventions.push_back(newIntervention);
    } else {
        throw InexistentService("There's no such service!");
    }
}

void HouseMaster::changeinterventionState(Intervention *intervention, processState state) {
    intervention->setProcessState(state);
}

std::vector<Intervention *> HouseMaster::getAssociatedInterventions(const std::string &id) {
    std::vector<Intervention*> retVec;
    for(const auto &intervention : _interventions){
        if (intervention->getClientId() == id || intervention->getCollabId() == id)
            retVec.push_back(intervention);
    }
    return retVec;
}

std::vector<std::pair<std::string, Collaborator *>> HouseMaster::sortCollaboratorsByScore() {
    std::vector<std::pair<std::string, Collaborator *>> temp;
    for (const auto &elem : _collaborators) {
        temp.emplace_back(elem);
    }
    std::sort(temp.begin(), temp.end(), colComparer);
    return temp;

}

Individual *HouseMaster::findByUsername(const std::string &username) {
    auto it = _usernameMap.find(username);
    if (it != _usernameMap.end()) {
        if (it->second.substr(0, 6) == "collab")
            return _collaborators[it->second];
        else if (it->second.substr(0, 6) == "client")
            return _clients[it->second];
    } else {
        // throw except
    }
}


void HouseMaster::assignColaborator(Intervention *intervention,
                                    const std::vector<std::pair<std::string, Collaborator *>> &orderedColls) {
    HouseMaster* hm = this;
    auto found = std::find_if(orderedColls.begin(), orderedColls.end(),
                              [&intervention, &hm](std::pair<std::string, Collaborator *> collaborator) -> bool {
                                  return collaborator.second->canDo(*hm, collaborator.second->getId(), intervention);
                              });

    if (found != orderedColls.end()) {
        intervention->setCollabId(found->first);
    } else
        throw UnavailableAppointment("No collaborators available to the desired date!");
}


