#include "HouseMaster.h"

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
std::vector<Intervention *> Individual::getAssociatedInterventions(HouseMaster &hm) const {
    return hm.getAssociatedInterventions(this->getId());
}

/**
 * @brief gets an individual's active interventions
 * @param hm the instance of housemaster
 * @return the active interventions
 */
std::vector<Intervention *> Individual::getAssociatedActiveInterventions(HouseMaster &hm) const {
    return hm.getAssociatedActiveInterventions(this->getId());
}

/**
 * @brief requests and processes an intervention according to the client's requirements
 * @param hm the instance of housemaster
 * @param date the desired date
 * @param service the service
 * @param forcePro
 * @param nrOfRooms the number of rooms
 */
void Client::requestIntervention(HouseMaster &hm, const Date &date, const std::string &service, bool forcePro,
                                 unsigned int nrOfRooms) const {
    hm.assignCollaborator(hm.addIntervention(date, service, forcePro, this->getId(), nrOfRooms),
                          hm.sortCollaboratorsByScore());
}

/**
 * @brief cancels an intervention
 * @param intervention to cancel
 */
void Client::cancelIntervention(Intervention *intervention) {
    HouseMaster::changeInterventionState(intervention, Canceled);
}

/**
 * @brief adds a new classification to a collaborator
 * @param hm the instance of housemaster
 * @param collabId the collaborator to classify
 * @param classification the classification to give the collab
 */
void Client::classifyCollaborator(HouseMaster &hm, const std::string &collabId, Classification classification) {
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
bool Collaborator::isAvailable(HouseMaster &hm, const std::string &collabId, Date start, Duration duration) {
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
bool Collaborator::canDo(HouseMaster &hm, const std::string &collabId, Intervention *intervention) {
    const Service *service = intervention->getService();
    Date start = intervention->getStartingTime();
    Duration duration = service->getDuration();
    return isAvailable(hm, collabId, start, duration) && canPreform(service->getName()) &&
           hasQualificationToPreform(intervention);
}

/**
 * @brief marks intervention as in progress
 * @param intervention in progress
 */
void Collaborator::markInterventionAsInProgress(Intervention *intervention) {
    HouseMaster::changeInterventionState(intervention, InProgress);
}

/**
 * @brief marks an intervention as completed
 * @param intervention to be completed
 */
void Collaborator::markInterventionAsComplete(Intervention *intervention) {
    HouseMaster::changeInterventionState(intervention, Complete);
}


// HouseMaster Methods

/**
 * @brief housemaster constructor
 */
HouseMaster::HouseMaster() : _availableServices(), _clients(), _collaborators(), _interventions(), _earnings() {

}

/**
 * @brief housemaster constructor from files
 * @param collaborators collabs info
 * @param clients clients info
 * @param services services info
 * @param earnings earnings info
 */
HouseMaster::HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services,
                         std::ifstream earnings) {

    // read services.txt
    for (std::string line; std::getline(services, line);) {
        std::stringstream lineStream(line);
        // name
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

        // check if everything is valid, if not throw an exception
    }

    // read collaborators.txt
    for (std::string line; std::getline(collaborators, line);) {
        std::stringstream lss(line);

        // name
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

        // check if everything is valid, if not throw an exception

        addCollaborator(collabServices, name, proStr == "yes", collabEarnings, score);
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

    // read earnings.txt
    for (std::string line; std::getline(earnings, line);) {
        _earnings = std::stof(line);
    }
}

/**
 * @brief getter
 * @return the collaborators
 */
std::map<std::string, Collaborator *> &HouseMaster::getCollaborators() {
    return _collaborators;
}

/**
 * @brief getter
 * @return the interventions
 */
std::vector<Intervention *> &HouseMaster::getInterventions() {
    return _interventions;
}

/**
 * @brief adds a service to the housemaster's catalogue
 * @param name name of the service
 * @param pro requires professional
 * @param basePrice base price
 * @param duration duration
 */
void HouseMaster::addAvailableService(const std::string &name, bool pro, float basePrice, const Duration &duration) {
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
void HouseMaster::addAvailablePaintService(const std::string &name, bool pro, float basePrice, const Duration& duration) {
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
void HouseMaster::removeAvailableService(const std::string &service) {

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
std::unordered_map<std::string, Service *> &HouseMaster::getAvailableServices() {
    return _availableServices;
}

/**
 * @brief adds a new collaborator to the housemaster
 * @param services services that the collaborator can preform
 * @param name name of the collaborator
 * @param pro is professional
 * @param earnings the earnings
 * @param score the score
 */
void HouseMaster::addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro,
                                  float earnings,
                                  Classification score) {
    auto collab = new Collaborator(services, name, pro, earnings, score);
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}


/**
 * @brief removes a collaborator from housemaster
 * @param id the id of the collaborator to remove
 */
void HouseMaster::removeCollaborator(const std::string &id) {
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
void HouseMaster::addClient(unsigned long nif, const std::string &name, bool premium) {
    auto it = std::find_if(_clients.begin(), _clients.end(), [&nif](const std::pair<std::string, Client *> &pair) {
        return pair.second->getNif() == nif;
    });
    if (it == _clients.end()) {
        auto client = new Client(nif, name, premium);
        _clients.insert({client->getId(), client});
        _usernameMap.insert({client->getId(), client->getId()});
    } else {
        throw HouseMaster::ExistentClient("Client already registred!");
    }
}


/**
 * @brief removes a client from housemaster
 * @param clientId the client to be removed
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
        }

    } else {
        throw NonexistentClient("Theres no such client!");
    }
}

/**
 * @brief getter
 * @return the clients
 */
std::map<std::string, Client *> &HouseMaster::getClients() {
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
Intervention *HouseMaster::addIntervention(const Date &start, const std::string &service, bool forcePro,
                                           const std::string &clientId, int nrOfRooms) {
    auto it = _availableServices.find(service);
    if (it == _availableServices.end()) throw NonexistentService("There's no such service!");
    auto newIntervention = new Intervention(start, _availableServices[service], forcePro, nrOfRooms);
    newIntervention->setClientId(clientId);
    newIntervention->calculateCost();
    _interventions.push_back(newIntervention);
    return newIntervention;
}

/**
 * @brief changes the intervention's state
 * @param intervention the intervention
 * @param state the desired state
 */
void HouseMaster::changeInterventionState(Intervention *intervention, processState state) {
    intervention->setProcessState(state);
}

/**
 * @brief processes the payment of an intervention
 * @param intervention the intervention
 */
void HouseMaster::processTransaction(Intervention *intervention) {
    float hmEarnings;
    hmEarnings = intervention->getCost() - (intervention->getCost() / float(1 + HouseMasterTax));
    getCollaborators()[intervention->getCollabId()]->calculateEarnings(hmEarnings);
    _earnings += hmEarnings;
    intervention->pay();
}

/**
 * @brief gets the interventions associated to an individual
 * @param id the individual's id
 * @return the interventions
 */
std::vector<Intervention *> HouseMaster::getAssociatedInterventions(const std::string &id) {
    std::vector<Intervention *> interventions;
    for (const auto &intervention : _interventions) {
        if (intervention->getClientId() == id || intervention->getCollabId() == id)
            interventions.push_back(intervention);
    }
    return interventions;
}


/**
 * @brief gets the active interventions associated to an individual
 * @param id the individual's id
 * @return the interventions
 */
std::vector<Intervention *> HouseMaster::getAssociatedActiveInterventions(const std::string &id) {
    std::vector<Intervention *> retVec;
    for (const auto &intervention : _interventions) {
        if ((intervention->getClientId() == id || intervention->getCollabId() == id) && intervention->isActive())
            retVec.push_back(intervention);
    }
    return retVec;
}

/**
 * @brief sorts the collaborators based on their performance
 * @return the sorted collaborators
 */
std::vector<std::pair<std::string, Collaborator *>> HouseMaster::sortCollaboratorsByScore() {
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
Individual *HouseMaster::findByUsername(const std::string &username) {
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
void HouseMaster::assignCollaborator(Intervention *intervention,
                                     const std::vector<std::pair<std::string, Collaborator *>> &orderedCollabs) {
    HouseMaster *hm = this;
    auto found = std::find_if(orderedCollabs.begin(), orderedCollabs.end(),
                              [&intervention, &hm](std::pair<std::string, Collaborator *> collaborator) -> bool {
                                  return collaborator.second->canDo(*hm, collaborator.second->getId(), intervention);
                              });

    if (found != orderedCollabs.end()) intervention->setCollabId(found->first);
    else throw UnavailableAppointment("No collaborators available to the desired date!");
}


/**
 * @brief saves the collaborators' info
 */
void HouseMaster::writeCollabsInfo() {
    std::ofstream collabFile;
    collabFile.open("../../data/collabs.txt");
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
    } else throw UnableToWriteFile("Unable to write in clients' file");
}

/**
 * @brief saves the clients' info
 */
void HouseMaster::writeClientsInfo() {
    std::ofstream clientsFile("../../data/clients.txt");
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
void HouseMaster::writeServicesInfo() {
    std::ofstream servicesFile("../../data/services.txt");
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
void HouseMaster::writeInterventionsInfo() {
    std::ofstream interventionsFile("../../data/history.txt", std::ios_base::app);
    time_t timeToday;
    time(&timeToday);
    interventionsFile << "\n\n" << asctime(localtime(&timeToday)) << "\n";
    if (interventionsFile.is_open()) {
        auto int_it = _interventions.begin();
        while (int_it != _interventions.end()) {
            interventionsFile << "Service: " << (*int_it)->getService()->getName() << ", from "
                              << (*int_it)->getStartingTime().getString()
                              << " to " << (*int_it)->getEndTime().getString() << ", to client "
                              << (*int_it)->getClientId() << " done by " << (*int_it)->getCollabId()
                              << ", cost: " << (*int_it)->getCost() << '\n';
            int_it++;
        }
        interventionsFile.close();
    } else throw UnableToWriteFile("Unable to write in interventions' file");
}

/**
 * @brief saves the financial state of housemaster
 */
void HouseMaster::writeFinancialInfo() const {
    std::ofstream finantialInfo("../../data/finances.txt", std::ios_base::trunc);
    if (finantialInfo.is_open()) {
        finantialInfo << _earnings << std::endl;
        finantialInfo.close();
    } else throw UnableToWriteFile("Unable to write in finances' file");
}

/**
 * @brief marks an intervention as complete
 * @param intervention the intervention
 */
void HouseMaster::markAsComplete(Intervention *intervention) {
    intervention->setProcessState(Complete);
}

/**
 * @brief getter
 * @return the earnings
 */
float HouseMaster::getEarnings() const {
    return _earnings;
}

/**
 * @brief exception for an impossible appointment
 * @param error_msg to show
 */
HouseMaster::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg) : std::logic_error(
        error_msg) {}

/**
 * @brief the exception for a nonexistent service
 * @param error_msg to show
 */
HouseMaster::NonexistentService::NonexistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for a service that already exists
 * @param error_msg to show
 */
HouseMaster::ExistentService::ExistentService(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for a nonexistent collaborator
 * @param error_msg to show
 */
HouseMaster::NonexistentCollab::NonexistentCollab(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for when trying to fire a collaborator with active interventions
 * @param error_msg to show
 */
HouseMaster::AssignedCollab::AssignedCollab(const std::string &error_msg) : std::logic_error(error_msg) {}

/**
 * @brief the exception for nonexistent clients
 * @param error_msg to show
 */
HouseMaster::NonexistentClient::NonexistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for when a client already exists
 * @param error_msg to show
 */
HouseMaster::ExistentClient::ExistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for nonexistent username
 * @param error_msg to show
 */
HouseMaster::NonexistentUsername::NonexistentUsername(const std::string &error_msg) : out_of_range(error_msg) {}

/**
 * @brief the exception for writing to file failures
 * @param error_msg to show
 */
HouseMaster::UnableToWriteFile::UnableToWriteFile(const std::string &error_msg) : std::ifstream::failure(error_msg) {}