#include "HouseMasterAffiliate.h"

#include <utility>

// unsigned int HouseMasterAffiliate::_idSeqAffiliate = 0; ??

/**
 * @brief compares two collaborators' score
 * @param a first collaborator
 * @param b second collaborator
 * @return true first has better score false otherwise
 */
bool scoreComparer(std::pair<std::string, Collaborator *> &a, std::pair<std::string, Collaborator *> &b) {
    return a.second->getScore() > b.second->getScore();
}

/**
 * @brief gets an individual's interventions
 * @param hm the instance of housemaster
 * @return the interventions
 */
std::unordered_set<Intervention *> Individual::getAssociatedInterventions(HouseMasterAffiliate &hm) const {
    return hm.getAssociatedInterventions(this->getId());
}

/**
 * @brief gets an individual's active interventions
 * @param hm the instance of housemaster
 * @return the active interventions
 */
std::unordered_set<Intervention *> Individual::getAssociatedActiveInterventions(HouseMasterAffiliate &hm) const {
    return hm.getAssociatedActiveInterventions(this->getId());
}

/**
 * @brief Changes username in username map
 * @param hm, the instance of housemaster
 */
void Individual::changeUsername(HouseMasterAffiliate &hm, std::string newUsername) const {
    hm.usernameMapChanger(this->getId(), std::move(newUsername));
}

/**
 * @brief requests and processes an intervention according to the client's requirements
 * @param hm the instance of housemaster
 * @param date the desired date
 * @param service the service
 * @param forcePro
 * @param nrOfRooms the number of rooms
 */
void Client::requestIntervention(HouseMasterAffiliate &hm, const Date &date, const std::string &service, bool forcePro,
                                 unsigned int nrOfRooms) const {
    hm.assignCollaborator(hm.addIntervention(date, service, forcePro, this->getId(), nrOfRooms),
                          hm.sortCollaboratorsByScore());
}

/**
 * @brief cancels an intervention
 * @param intervention to cancel
 */
void Client::cancelIntervention(Intervention *intervention) {
    HouseMasterAffiliate::changeInterventionState(intervention, Canceled);
}

/**
 * @brief adds a new classification to a collaborator
 * @param hm the instance of housemaster
 * @param collabId the collaborator to classify
 * @param classification the classification to give the collab
 */
void
Client::classifyCollaborator(HouseMasterAffiliate &hm, const std::string &collabId, Classification classification) {
    hm.getCollaborators()[collabId]->addClassification(classification);
}


/**
 * @brief checks if a collaborator is available for a service
 * @param hm the instance of housemaster
 * @param collabId the id of the collaborator
 * @param start of intervention
 * @param duration of intervention
 * @return true if available otherwise false
 */
bool Collaborator::isAvailable(HouseMasterAffiliate &hm, const std::string &collabId, Date start, Duration duration) {
    auto interventions = hm.getAssociatedActiveInterventions(collabId);
    return std::all_of(interventions.begin(), interventions.end(), [&start, &duration](Intervention *intervention) {
        return !intervention->conflictsWith(start, duration);
    });
}


/**
 * @brief checks if a collaborator an do an intervention
 * @param hm the instance of housemaster
 * @param collabId the collaborator's id
 * @param intervention the intervention
 * @return true if can do false otherwise
 */
bool Collaborator::canDo(HouseMasterAffiliate &hm, const std::string &collabId, Intervention *intervention) {
    const Service *service = intervention->getService();
    Date start = intervention->getStartingTime();
    Duration duration = service->getDuration();
    return isAvailable(hm, collabId, start, duration) && canPreform(service->getName()) &&
           hasQualificationToPreform(intervention);
}


/**
 * @brief marks an intervention as completed
 * @param intervention to be completed
 */
void Collaborator::markInterventionAsComplete(Intervention *intervention) {
    HouseMasterAffiliate::changeInterventionState(intervention, Complete);
}


// HouseMaster Methods

/**
 * @brief housemaster constructor
 */
HouseMasterAffiliate::HouseMasterAffiliate()
        : _availableServices(), _clients(), _usernameMap(), _collaborators(), _interventions(), _earnings(),
          _responsible() {

}

/**
 * @brief housemaster constructor from files
 * @param collaborators collabs info
 * @param clients clients info
 * @param services services info
 * @param earnings earnings info
 */
HouseMasterAffiliate::HouseMasterAffiliate(std::ifstream usernames, std::ifstream collaborators, std::ifstream clients,
                                           std::ifstream services, std::ifstream history, std::string location,
                                           const std::string &responsible, const std::string &hmName, float finances)
        : _name(hmName), _location(std::move(location)), _responsible(Admin(responsible)), _earnings(finances) {

    // read services.txt
    for (std::string line; std::getline(services, line);) {
        std::stringstream lineStream(line);
        // hmName
        std::string name;
        std::getline(lineStream, name, ',');
        // pro
        std::string proStr;
        std::getline(lineStream, proStr, ',');
        bool pro = proStr == "yes";
        // price
        std::string priceStr;
        std::getline(lineStream, priceStr, ',');
        float price = std::stof(priceStr);
        // duration
        std::string durationStr;
        std::getline(lineStream, durationStr, ',');
        Duration duration(durationStr);
        // category
        std::string category{};
        std::getline(lineStream, category, ',');
        if (category == "default") {
            addAvailableService(name, pro, price, duration);
        } else if (category == "painting") {
            addAvailablePaintService(name, pro, price, duration);
        }
    }

    // read collaborators.txt
    for (std::string line; std::getline(collaborators, line);) {
        std::stringstream lss(line);

        //check if a collaborator works for that affiliate
        std::string affiliateName{};
        std::getline(lss, affiliateName, ',');
        if (affiliateName == hmName) {
            // hmName
            std::string name{};
            std::getline(lss, name, ',');
            // pro
            std::string proStr{};
            std::getline(lss, proStr, ',');
            // earnings
            std::string earnStr{};
            std::getline(lss, earnStr, ',');
            float collabEarnings = std::stof(earnStr);
            // score
            std::string scoreStr{};
            std::getline(lss, scoreStr, ',');
            auto score = Classification(std::stoi(scoreStr));
            // services
            std::string serviceName{};
            std::vector<std::string> collabServices;

            while (std::getline(lss, serviceName, ',')) {
                collabServices.push_back(serviceName);
            }

            addCollaborator(collabServices, name, proStr == "yes", collabEarnings, score, affiliateName);
        } else continue;
    }


    // read clients.txt
    for (std::string line; std::getline(clients, line);) {
        std::stringstream lss(line);
        //check if it is a affiliate's client
        std::string affiliate_name{};
        std::getline(lss, affiliate_name, ',');
        if (affiliate_name == hmName) {
            std::string name;
            std::string nifStr{};
            std::string premiumStr{};
            std::getline(lss, name, ',');
            std::getline(lss, nifStr, ',');
            std::getline(lss, premiumStr, ',');

            addClient(std::stoul(nifStr), name, premiumStr == "yes", affiliate_name);
        } else continue;
    }

    // read usernames.txt
    for (std::string line; std::getline(usernames, line);) {
        std::stringstream lss(line);
        std::string username;
        std::string id;
        std::getline(lss, username, ',');
        std::getline(lss, id, ',');
        _usernameMap.emplace(std::pair<std::string, std::string>(username, id));
    }

    // read history.txt
    for (std::string line; std::getline(history, line);) {
        if (line.front() == '@') continue;
        else if (line.front() == ' ') break;
        std::stringstream lineStream(line);
        std::string serviceStr{}, clientId{}, collabId{}, start{}, forcePro{}, state{}, cost{}, nrRooms{};
        std::getline(lineStream, serviceStr, ',');
        Service *service = _availableServices[serviceStr];
        std::getline(lineStream, clientId, ',');
        std::getline(lineStream, collabId, ',');
        std::getline(lineStream, start, ',');
        std::getline(lineStream, forcePro, ',');
        std::getline(lineStream, state, ',');
        std::getline(lineStream, cost, ',');
        std::getline(lineStream, nrRooms, ',');
        auto *intervention = new Intervention(Date(start), service, forcePro == "1", std::stoi(nrRooms),
                                              processState(std::stoi(state)), std::stof(cost),
                                              collabId, clientId);
        _interventions.insert(intervention);
    }

}

/**
 * @brief getter
 * @return the collaborators
 */
std::map<std::string, Collaborator *> &HouseMasterAffiliate::getCollaborators() {
    return _collaborators;
}

/**
 * @brief getter
 * @return the interventions
 */
std::unordered_set<Intervention *> &HouseMasterAffiliate::getInterventions() {
    return _interventions;
}

/**
 * @brief adds a service to the housemaster's catalogue
 * @param name name of the service
 * @param pro requires professional
 * @param basePrice base price
 * @param duration duration
 */
void HouseMasterAffiliate::addAvailableService(const std::string &name, bool pro, float basePrice,
                                               const Duration &duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        auto service = new Service(name, pro, basePrice, duration);
        _availableServices.insert({name, service});
    }
}


/**
 * @brief adds a painting service to the housemaster's catalogue
 * @param name name of the service
 * @param pro requires professional
 * @param basePrice base price
 * @param duration duration
 */
void HouseMasterAffiliate::addAvailablePaintService(const std::string &name, bool pro, float basePrice,
                                                    const Duration &duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw ExistentService("A service with the same name already exists!");
    else {
        auto service = new Painting(name, pro, basePrice, duration);
        _availableServices.insert({name, service});
    }
}

/**
 * @brief removes a service from housemaster's catalogue
 * @param service name of the service
 */
void HouseMasterAffiliate::removeAvailableService(const std::string &service) {

    auto it = _availableServices.find(service);
    if (it != _availableServices.end()) {
        delete it->second;
        _availableServices.erase(it);
    } else {
        throw NonexistentService("There's no such service!");
    }
}

/**
 * @brief getter
 * @return available services
 */
std::unordered_map<std::string, Service *> &HouseMasterAffiliate::getAvailableServices() {
    return _availableServices;
}

/**
 * @brief Manipulates the username map
 * @param string with the user id
 */
void HouseMasterAffiliate::usernameMapChanger(std::string id, std::string newUsername) {
    auto user = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                             [&id](const std::pair<std::string, std::string> &mapElem) {
                                 return mapElem.second == id;
                             });

    if (user != _usernameMap.end()) {
        auto alreadyInUse = std::find_if_not(_usernameMap.begin(), _usernameMap.end(),
                                             [&newUsername](const std::pair<std::string, std::string> &username) {
                                                 return newUsername != username.first;
                                             });

        if (alreadyInUse == _usernameMap.end()) {
            _usernameMap.erase(user);
            _usernameMap.emplace(std::pair<std::string, std::string>(newUsername, id));
        } else {
            throw UsernameAlreadyInUse("Username in use, choose another!");
        }
    } else {}
}

/**
 * @brief adds a new collaborator to the housemaster
 * @param services services that the collaborator can preform
 * @param name name of the collaborator
 * @param pro is professional
 * @param earnings the earnings
 * @param score the score
 * @param affiliate the affiliate's name
 */
void HouseMasterAffiliate::addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro,
                                           float earnings,
                                           Classification score, std::string affiliate) {
    auto collab = new Collaborator(services, name, pro, earnings, score, std::move(affiliate));
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}


/**
 * @brief removes a collaborator from housemaster
 * @param id the id of the collaborator to remove
 */
void HouseMasterAffiliate::removeCollaborator(const std::string &id) {
    auto it = _collaborators.find(id);
    auto usernameIt = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                                   [&id](const std::pair<std::string, std::string> &mapped) {
                                       return mapped.second == id;
                                   });
    if (it != _collaborators.end()) {
        if (usernameIt != _usernameMap.end()) {
            if (getAssociatedActiveInterventions(id).empty()) {
                _collaborators.erase(it);
                _usernameMap.erase(usernameIt);
            } else {
                throw AssignedCollab("Collaborator still has incomplete Interventions!");
            }
        } else {
            throw NonexistentUsername("This username does not exist!");
        }
    } else {
        throw NonexistentCollab("There's no such collab!");
    }
}

/**
 * @brief adds a new client to the housemaster
 * @param nif tax id
 * @param name name
 * @param premium is premium
 */
void HouseMasterAffiliate::addClient(unsigned long nif, const std::string &name, bool premium, std::string affiliate) {

    auto it = std::find_if(_clients.begin(), _clients.end(), [&nif](const std::pair<std::string, Client *> &pair) {
        return pair.second->getNif() == nif;
    });
    if (it == _clients.end()) {
        auto client = new Client(nif, name, premium, std::move(affiliate));
        _clients.insert({client->getId(), client});
        _usernameMap.insert({client->getId(), client->getId()});
    } else {
        throw HouseMasterAffiliate::ExistentClient("Client already registred!");
    }
}


/**
 * @brief removes a client from housemaster
 * @param clientId the client to be removed
 */
void HouseMasterAffiliate::removeClient(const std::string &clientId) {

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
        }

    } else {
        throw NonexistentClient("Theres no such client!");
    }
}

/**
 * @brief getter
 * @return the clients
 */
std::map<std::string, Client *> HouseMasterAffiliate::getClients() const {
    return _clients;
}

/**
 * @brief adds an intervention
 * @param start start of the intervention
 * @param service the service
 * @param forcePro
 * @param clientId the clients id
 * @param nrOfRooms number of rooms in case of painting
 * @return the intervention
 */
Intervention *HouseMasterAffiliate::addIntervention(const Date &start, const std::string &service, bool forcePro,
                                                    const std::string &clientId, unsigned int nrOfRooms) {
    auto it = _availableServices.find(service);
    if (it == _availableServices.end()) throw NonexistentService("There's no such service!");
    auto newIntervention = new Intervention(start, _availableServices[service], forcePro, nrOfRooms, Active, 0, "",
                                            clientId);
    _interventions.insert(newIntervention);
    return newIntervention;
}

/**
 * @brief changes the intervention's state
 * @param intervention the intervention
 * @param state the desired state
 */
void HouseMasterAffiliate::changeInterventionState(Intervention *intervention, processState state) {
    intervention->setProcessState(state);
}

/**
 * @brief processes the payment of an intervention
 * @param intervention the intervention
 */
void HouseMasterAffiliate::processTransaction(Intervention *intervention) {
    float hmEarnings;
    hmEarnings = intervention->getCost() - (intervention->getCost() / float(1 + HouseMasterTax));
    getCollaborators()[intervention->getCollabId()]->calculateEarnings(hmEarnings);
    _earnings += hmEarnings;
}

/**
 * @brief gets the interventions associated to an individual
 * @param id the individual's id
 * @return the interventions
 */
std::unordered_set<Intervention *> HouseMasterAffiliate::getAssociatedInterventions(const std::string &id) {
    std::unordered_set<Intervention *> interventions;
    for (const auto &intervention : _interventions) {
        if (intervention->getClientId() == id || intervention->getCollabId() == id)
            interventions.insert(intervention);
    }
    return interventions;
}


/**
 * @brief gets the active interventions associated to an individual
 * @param id the individual's id
 * @return the interventions
 */
std::unordered_set<Intervention *> HouseMasterAffiliate::getAssociatedActiveInterventions(const std::string &id) {
    std::unordered_set<Intervention *> retVec;
    for (const auto &intervention : _interventions) {
        if ((intervention->getClientId() == id || intervention->getCollabId() == id) && intervention->isActive())
            retVec.insert(intervention);
    }
    return retVec;
}

/**
 * @brief gets the non active interventions associated to an individual
 * @param id the individual's id
 * @return the interventions
 */
std::unordered_set<Intervention *> HouseMasterAffiliate::getAssociatedPastInterventions(const std::string &id) {
    std::unordered_set<Intervention *> retVec;
    for (const auto &intervention : _interventions) {
        if ((intervention->getClientId() == id || intervention->getCollabId() == id) && !intervention->isActive())
            retVec.insert(intervention);
    }
    return retVec;
}

/**
 * @brief sorts the collaborators based on their performance
 * @return the sorted collaborators
 */
std::vector<std::pair<std::string, Collaborator *>> HouseMasterAffiliate::sortCollaboratorsByScore() {
    std::vector<std::pair<std::string, Collaborator *>> temp;
    for (const auto &elem : _collaborators) {
        temp.emplace_back(elem);
    }
    std::sort(temp.begin(), temp.end(), scoreComparer);
    return temp;
}

/**
 * @brief finds an individual by their username
 * @param username the username
 * @return the individual
 */
Individual *HouseMasterAffiliate::findByUsername(const std::string &username) {
    auto it = _usernameMap.find(username);
    if (it == _usernameMap.end()) throw NonexistentUsername("This username does not exist!");
    if (it->second.substr(0, 6) == "collab") return _collaborators[it->second];
    else if (it->second.substr(0, 6) == "client") return _clients[it->second];
    else return nullptr;
}

/**
 * @brief assigns a collaborator to an intervention
 * @param intervention the intervention
 * @param orderedCollabs the collaborators
 */
void HouseMasterAffiliate::assignCollaborator(Intervention *intervention,
                                              const std::vector<std::pair<std::string, Collaborator *>> &orderedCollabs) {
    HouseMasterAffiliate *hm = this;
    auto found = std::find_if(orderedCollabs.begin(), orderedCollabs.end(),
                              [&intervention, &hm](std::pair<std::string, Collaborator *> collaborator) -> bool {
                                  return collaborator.second->canDo(*hm, collaborator.second->getId(), intervention);
                              });

    if (found != orderedCollabs.end()) intervention->setCollabId(found->first);
    else throw UnavailableAppointment("No collaborators available to the desired date!");
}

/**
 * @brief Saves usernameMap
 */
void HouseMasterAffiliate::writeUsernameMap() {
    std::ofstream usernameMapFile("../data/usernames.txt");
    if (usernameMapFile.is_open()) {
        auto usernameIt = _usernameMap.begin();
        while (usernameIt != _usernameMap.end()) {
            usernameMapFile << usernameIt->first << ',' << usernameIt->second << std::endl;
            usernameIt++;
        }
        usernameMapFile.close();
    } else throw UnableToWriteFile("Unable to write in usernames' file");
}

/**
 * @brief saves the collaborators' info
 */
void HouseMasterAffiliate::writeCollabsInfo() {
    std::ofstream collabFile;
    collabFile.open("../data/collabs.txt");
    if (collabFile.is_open()) {
        auto collab_it = _collaborators.begin();
        while (collab_it != _collaborators.end()) {
            collabFile << collab_it->second->getName();
            if (collab_it->second->isPro()) { collabFile << ",yes,"; } else { collabFile << ",no,"; }
            collabFile << collab_it->second->getEarnings() << ',' << collab_it->second->getScore() << ',';
            for (size_t i = 0; i < collab_it->second->getServices().size(); i++) {
                if (i == collab_it->second->getServices().size() - 1)
                    collabFile << collab_it->second->getServices()[i];
                else collabFile << collab_it->second->getServices()[i] << ",";
            }
            collabFile << '\n';
            collab_it++;
        }
        collabFile.close();
    } else throw UnableToWriteFile("Unable to write in collabs' file");
}

/**
 * @brief saves the clients' info
 */
void HouseMasterAffiliate::writeClientsInfo() {
    std::ofstream clientsFile("../data/clients.txt");
    if (clientsFile.is_open()) {
        auto client_it = _clients.begin();
        while (client_it != _clients.end()) {
            clientsFile << client_it->second->getName() << "," << client_it->second->getNif();
            if (client_it->second->isPremium()) { clientsFile << ",yes"; } else { clientsFile << ",no"; }
            clientsFile << '\n';
            client_it++;
        }
        clientsFile.close();
    } else throw UnableToWriteFile("Unable to write in clients' file");
}

/**
 * @brief saves the services' info
 */
void HouseMasterAffiliate::writeServicesInfo() {
    std::ofstream servicesFile("../data/services.txt");
    if (servicesFile.is_open()) {
        auto service_it = _availableServices.begin();
        while (service_it != _availableServices.end()) {

            Service *sv = (*service_it).second;
            auto pt = dynamic_cast<Painting *>(sv);
            servicesFile << service_it->second->getName() << ",";
            if (service_it->second->getPro()) { servicesFile << "yes"; } else { servicesFile << "no"; }
            servicesFile << "," << service_it->second->getBasePrice() << ","
                         << service_it->second->getDuration().getString();
            if (pt) {
                servicesFile << ",painting";
            } else {
                servicesFile << ",default";
            }
            servicesFile << '\n';
            service_it++;
        }
        servicesFile.close();
    } else throw UnableToWriteFile("Unable to write in services' file");
}

/**
 * @brief saves the interventions' history info
 */
void HouseMasterAffiliate::writeInterventionsInfo() {
    std::ofstream interventionsFile("../data/history.txt");
    time_t timeToday;
    time(&timeToday);
    interventionsFile << '@' << asctime(localtime(&timeToday));
    if (interventionsFile.is_open()) {
        auto int_it = _interventions.begin();
        while (int_it != _interventions.end()) {
            interventionsFile << (*int_it)->getService()->getName() << ',' << (*int_it)->getClientId() << ','
                              << (*int_it)->getCollabId() << ',' << (*int_it)->getStartingTime().getString() << ','
                              << (*int_it)->getForcePro() << ',' << (*int_it)->getProcessState() << ','
                              << (*int_it)->getCost();
            auto *service = (*int_it)->getService();
            auto painting = dynamic_cast<Painting *>(service);
            if (painting) interventionsFile << ',' << painting->getRoomNumber(); else interventionsFile << ",0";
            interventionsFile << '\n';
            int_it++;
        }
        interventionsFile.close();
    } else throw UnableToWriteFile("Unable to write in interventions' file");
}


/**
 * @brief marks an intervention as complete
 * @param intervention the intervention
 */
void HouseMasterAffiliate::markAsComplete(Intervention *intervention) {
    intervention->setProcessState(Complete);
}

/**
 * @brief getter
 * @return the earnings
 */
float HouseMasterAffiliate::getEarnings() const {
    return _earnings;
}

/**
 * @brief getter
 * @return all active interventions
 */
std::unordered_set<Intervention *> HouseMasterAffiliate::getAllActiveInterventions() {
    std::unordered_set<Intervention *> result{};
    for (const auto &intervention : _interventions) {
        if (intervention->isActive()) result.insert(intervention);
    }
    return result;
}

/**
 * @brief getter
 * @return all past interventions
 */
std::unordered_set<Intervention *> HouseMasterAffiliate::getAllPastInterventions() {
    std::unordered_set<Intervention *> result{};
    for (const auto &intervention : _interventions) {
        if (!intervention->isActive()) result.insert(intervention);
    }
    return result;
}

/**
 * @brief getter
 * @return affiliate's name
 */
std::string HouseMasterAffiliate::getAffiliateName() const {
    return _name;
}

/**
 * @brief getter
 * @return admin
 */
Admin HouseMasterAffiliate::getAdmin() const {
    return _responsible;
}

/**
 * @brief getter
 * @return locality
 */
std::string HouseMasterAffiliate::getLocation() const {
    return _location;
}

/**
 * @brief getter
 * @return responsible
 */
std::string HouseMasterAffiliate::getResponsible() const {
    return _responsible.getName();
}



/**
 * @brief exception for an impossible appointment
 * @param error_msg to show
 */
HouseMasterAffiliate::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg) : std::logic_error(
        error_msg) {}

/**
 * @brief the exception for a nonexistent service
 * @param error_msg to show
 */
HouseMasterAffiliate::NonexistentService::NonexistentService(const std::string &error_msg) : std::out_of_range(
        error_msg) {}

/**
 * @brief the exception for a service that already exists
 * @param error_msg to show
 */
HouseMasterAffiliate::ExistentService::ExistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brirf Exception thrown when trying to change username to another already in use
 * @param error_msg to show
 */
HouseMasterAffiliate::UsernameAlreadyInUse::UsernameAlreadyInUse(const std::string &error_msg) : std::logic_error(
        error_msg) {};

/**
 * @brief the exception for a nonexistent collaborator
 * @param error_msg to show
 */
HouseMasterAffiliate::NonexistentCollab::NonexistentCollab(const std::string &error_msg) : std::out_of_range(
        error_msg) {}

/**
 * @brief the exception for when trying to fire a collaborator with active interventions
 * @param error_msg to show
 */
HouseMasterAffiliate::AssignedCollab::AssignedCollab(const std::string &error_msg) : std::logic_error(error_msg) {}

/**
 * @brief the exception for nonexistent clients
 * @param error_msg to show
 */
HouseMasterAffiliate::NonexistentClient::NonexistentClient(const std::string &error_msg) : std::out_of_range(
        error_msg) {}

/**
 * @brief the exception for when a client already exists
 * @param error_msg to show
 */
HouseMasterAffiliate::ExistentClient::ExistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for nonexistent username
 * @param error_msg to show
 */
HouseMasterAffiliate::NonexistentUsername::NonexistentUsername(const std::string &error_msg) : out_of_range(
        error_msg) {}

/**
 * @brief the exception for writing to file failures
 * @param error_msg to show
 */
HouseMasterAffiliate::UnableToWriteFile::UnableToWriteFile(const std::string &error_msg) : std::ifstream::failure(
        error_msg) {}


bool HouseMasterAffiliate::operator<(const HouseMasterAffiliate &hma) const {
    if (_responsible.getName() == hma.getAdmin().getName()) {
        if (getClients().size() == hma.getClients().size()) {
            return (_location < hma.getLocation());
        } else return (_clients.size() < hma.getClients().size());
    } else return (_responsible.getName() < hma.getAdmin().getName());
}