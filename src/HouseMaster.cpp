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

HouseMaster::NonexistentUsername::NonexistentUsername(const std::string &error_msg) : out_of_range(error_msg){}

HouseMaster::UnableToWriteFile::UnableToWriteFile(const std::string &error_msg) : std::ifstream::failure(error_msg) {}

std::vector<Intervention*> Individual::getAssociatedInterventions(HouseMaster &hm) const{
    return hm.getAssociatedInterventions(this->getId());
}

std::vector<Intervention*> Individual::getAssociatedActiveInterventions(HouseMaster &hm) const{
    return hm.getAssociatedActiveInterventions(this->getId());
}

void Client::requestIntervention(HouseMaster &hm, const date &_date, const std::string &type, bool forcePro, int nrOfRooms) const{
    hm.assignColaborator(hm.addIntervention(_date, type, forcePro, this->getId(), nrOfRooms), hm.sortCollaboratorsByScore());
}

void Client::cancelIntervention(Intervention * intervention) {
    HouseMaster::changeinterventionState(intervention, Canceled);
}

void Client::classifyCollaborator(HouseMaster &hm, const std::string &collabId, Classification classification) {
    hm.getCollaborators()[collabId]->addClassification(classification);
}

bool Collaborator::isAvailable(HouseMaster &hm, const std::string &collabId, date start, duration duration) {
    for (const auto &intervention : hm.getAssociatedActiveInterventions(collabId)) {
        if (intervention->conflictsWith(start, duration)) {
            return false;
        }
    }
    return true;
}

bool Collaborator::canDo(HouseMaster& hm, const std::string &collabId, Intervention *intervention) {
    const Service *service = intervention->getService();
    date start = intervention->getStartingTime();
    duration duration = service->getDuration();
    return isAvailable(hm, collabId, start, duration) && canPreform(service->getName()) && hasQualificationToPreform(intervention);
}

void Collaborator::markInterventionAsInProgress(Intervention *a) {
    HouseMaster::changeinterventionState(a, InProgress);
}

void Collaborator::markInterventionAsComplete(Intervention *a) {
    HouseMaster::changeinterventionState(a, Complete);
}


// HouseMaster Methods
HouseMaster::HouseMaster() : _availableServices(), _clients(), _collaborators(), _interventions(), _earnings() {}

HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services) : _earnings() {
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
        duration duration(durationStr);
        // category
        std::string category{};
        std::getline(lss, category, ',');
        if (category=="default") {
            addAvailableService(name, pro, price, duration);
        } else if (category=="painting") {
            addAvailablePaintService(name, pro, price, duration);
        }

        // check if everything is valid, if not throw an exception
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

        // check if everything is valid, if not throw an exception

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

        // check if everything is valid, if not throw an exception

        addClient(std::stoul(nifStr), name, premiumStr == "yes");
    }
}

std::unordered_map<std::string, Collaborator *> &HouseMaster::getCollaborators() {
    return _collaborators;
}

std::vector<Intervention *> &HouseMaster::getInterventions() {
    return _interventions;
}

void HouseMaster::addAvailableService(const std::string &name, bool pro, float basePrice, duration duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        auto service = new Service(name, pro, basePrice, duration);
        _availableServices.insert({name, service});
    }
}

void HouseMaster::addAvailablePaintService(const std::string &name, bool pro, float basePrice, duration duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        Painting* service = new Painting(name, pro, basePrice, duration);
        _availableServices.insert({name, service});
    }
}

void HouseMaster::addAvailableService(Service* service) {
    if (_availableServices.find(service->getName()) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        _availableServices.insert({service->getName(), service});
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

void HouseMaster::addCollaborator(Collaborator *collab) {
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}

void HouseMaster::addCollaborator(const std::vector<std::string> &functions, const std::string &name, bool pro) {
    auto collab = new Collaborator(functions, name, pro);
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}

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
            throw NonexistentUsername("This username does not exist!");
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

void HouseMaster::addClient(Client *client) {

    // nif name premium
    auto it = std::find_if(_clients.begin(), _clients.end(), [&client](const std::pair<std::string, Client*> &pair){
        return (pair.second->getNif() == client->getNif());
    });
    if(it == _clients.end()) {
        _clients.insert({client->getId(), client});
        _usernameMap.insert({client->getId(), client->getId()});
    }
    else{
        throw HouseMaster::ExistentClient("Client already registred!");
    }
}

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

/*
 * void HouseMaster::addClient(unsigned int nif, const std::string &name, bool premium) {
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
 */

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
            throw NonexistentUsername("This username does not exist!");
            //throws except
        }

    } else {
        throw InexistentClient("Theres no such client!");
    }

}

std::unordered_map<std::string, Client *> &HouseMaster::getClients() {
    return _clients;
}

/*
Intervention* HouseMaster::addIntervention(const date &appointment, const std::string &type, bool forcePro, const std::string &clientId) {
    auto it = _availableServices.find(type);
    if (it != _availableServices.end()) {
        auto newIntervention = new Intervention(appointment, *_availableServices[type], forcePro);
        newIntervention->setClientId(clientId);
        newIntervention->calculateCost();
        _interventions.push_back(newIntervention);
        return newIntervention;
    } else {
        throw InexistentService("There's no such service!");
    }
}
*/

Intervention* HouseMaster::addIntervention(const date &appointment, const std::string &type, bool forcePro,
                                           const std::string &clientId, unsigned int nrOfRooms) {
    auto it = _availableServices.find(type);
    if (it != _availableServices.end()) {
        auto newIntervention = new Intervention(appointment, *_availableServices[type], forcePro, nrOfRooms);
        newIntervention->setClientId(clientId);
        newIntervention->calculateCost();
        _interventions.push_back(newIntervention);
        return newIntervention;
    } else {
        throw InexistentService("There's no such service!");
    }
}

void HouseMaster::changeinterventionState(Intervention *intervention, processState state) {
    intervention->setProcessState(state);
}


void HouseMaster::processTransaction(Intervention *intervention) {
    float hmEarnings;
    intervention->calculateCost();
    hmEarnings = intervention->getCost() - (intervention->getCost() / float(1 + HouseMasterTax));
    getCollaborators()[intervention->getCollabId()]->calculateEarnings(hmEarnings);
    _earnings += hmEarnings;
    intervention->pay();
}

std::vector<Intervention *> HouseMaster::getAssociatedInterventions(const std::string &id) {
    std::vector<Intervention*> retVec;
    for(const auto &intervention : _interventions){
        if (intervention->getClientId() == id || intervention->getCollabId() == id)
            retVec.push_back(intervention);
    }
    return retVec;
}

std::vector<Intervention *> HouseMaster::getAssociatedActiveInterventions(const std::string &id) {
    std::vector<Intervention*> retVec;
    for(const auto &intervention : _interventions){
        if ((intervention->getClientId() == id || intervention->getCollabId() == id) && intervention->isActive())
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
        throw NonexistentUsername("This username does not exist!");
        // throw except

    }
    return nullptr;
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



void HouseMaster::writeCollabsInfo()
{
    std::ofstream collabFile;
    collabFile.open("../../data/collabs.txt");
    if (collabFile.is_open())
    {
        auto collab_it = _collaborators.begin();
        while (collab_it != _collaborators.end())
        {
            collabFile << collab_it->second->getName();
            if (collab_it->second->isPro()) { collabFile << ",yes,"; } else { collabFile << ",no,"; }
            for (size_t i = 0; i < collab_it->second->getServices().size(); i++)
            {
                if (i == collab_it->second->getServices().size() - 1)
                    collabFile << collab_it->second->getServices()[i];
                else collabFile << collab_it->second->getServices()[i] << ",";
            }
            collabFile << '\n';
            collab_it++;
        }
        collabFile.close();
    } else throw UnableToWriteFile("Unable to write in clients' file");
}


void HouseMaster::writeClientsInfo()
{
    std::ofstream clientsFile("../../data/clients.txt");
    if (clientsFile.is_open())
    {
        auto client_it = _clients.begin();
        while (client_it != _clients.end())
        {
            clientsFile << client_it->second->getName() << "," << client_it->second->getNif();
            if (client_it->second->isPremium()) {clientsFile << ",yes";} else {clientsFile << ",no";}
            clientsFile << '\n';
            client_it ++;
        }
        clientsFile.close();
    }
    else throw UnableToWriteFile("Unable to write in clients' file");
}

void HouseMaster::writeServicesInfo() {
    std::ofstream servicesFile("../../data/services.txt");
    if (servicesFile.is_open()) {
        auto service_it = _availableServices.begin();
        while (service_it != _availableServices.end()) {

            Service* sv = (*service_it).second;
            Painting* pt = dynamic_cast<Painting*>(sv);
            servicesFile << service_it->second->getName() << ",";
            if (service_it->second->getPro()) {servicesFile << "yes";} else {servicesFile << "no";}
            servicesFile << "," << service_it->second->getBasePrice() << "," << service_it->second->getDuration().durationToStr();
            if (pt) {
                servicesFile << ",painting";
            } else {
                servicesFile << ",default";
            }
            servicesFile << '\n';
            service_it++;
        }
        servicesFile.close();
    }
    else throw UnableToWriteFile("Unable to write in services' file");
}

void HouseMaster::writeInterventionsInfo()
{
    std::ofstream interventionsFile("../../data/history.txt", std::ios_base::app);
    time_t timeToday;
    time (&timeToday);
    interventionsFile << "\n\n" << asctime(localtime(&timeToday)) << "\n";
    if (interventionsFile.is_open())
    {
        auto int_it = _interventions.begin();
        while (int_it != _interventions.end())
        {
            interventionsFile << "Service: " << (*int_it)->getService()->getName() << ", from " << (*int_it)->getStartingTime().dateToStr()
            << " to " << (*int_it)->getEndTime().dateToStr() << ", to client " << (*int_it)->getClientId() << " done by " << (*int_it)->getCollabId()
            << ", cost: " << (*int_it)->getCost() << '\n';
            int_it++;
        }
        interventionsFile.close();
    }
    else throw UnableToWriteFile("Unable to write in interventions' file");
}

void HouseMaster::markAsComplete(Intervention *intervention) {
    intervention->setProcessState(Complete);
}

float HouseMaster::getEarnings() const {
    return _earnings;
}
