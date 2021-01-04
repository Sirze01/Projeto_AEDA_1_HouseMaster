#include "HouseMaster.h"

#include <utility>

// Constructors and Destructor
/**
 * @brief housemaster constructor
 */
HouseMaster::HouseMaster() : _affiliates(HouseMasterAffiliate(this)) {

}

/**
 * @brief housemaster constructor from files
 * @param affiliates affiliates' info
 */
HouseMaster::HouseMaster(std::ifstream affiliates) : _affiliates(HouseMasterAffiliate(this)) {
    for (std::string line; std::getline(affiliates, line);) {
        std::stringstream lss(line);
        // name
        std::string name{};
        std::getline(lss, name, ',');
        // responsible
        std::string responsible{};
        std::getline(lss, responsible, ',');
        // location
        std::string location{};
        std::getline(lss, location, ',');
        // finances
        std::string finances{};
        std::getline(lss, finances, ',');
        float earnings = stof(finances);
        auto h1 = HouseMasterAffiliate(this, std::ifstream("../data/usernames.txt"),
                                       std::ifstream("../data/collabs.txt"),
                                       std::ifstream("../data/clients.txt"),
                                       std::ifstream("../data/services.txt"),
                                       std::ifstream("../data/history.txt"),
                                       std::ifstream("../data/responsibles.txt"),
                                       location, name, earnings);
        _locations.insert(location);
        registerAffiliate(h1);
    }
    BSTItrIn<HouseMasterAffiliate> current(_affiliates);
    for (; !current.isAtEnd(); current.advance()) {
        for (const auto &i : _responsibles) {
            for (const auto &loc : i.second->getAffiliates()) {
                if (loc == current.retrieve().getLocation()) {
                    auto toChange = current.retrieve();
                    toChange.setAdmin(*(i.second));
                    _affiliates.remove(current.retrieve());
                    _affiliates.insert(toChange);
                }
            }
        }
    }
}


// Affiliates Manip
/**
 * @brief getter
 * @return the affiliates
 */
BST<HouseMasterAffiliate> &HouseMaster::getAffiliates() {
    return _affiliates;
}

/**
 * @brief adds a new affiliate to the housemaster
 * @param affiliate the affiliate
 */
void HouseMaster::registerAffiliate(const HouseMasterAffiliate &affiliate) {
    _affiliates.insert(affiliate);
    for (const auto &client : getClients()) {
        _clientContacts.insert(client.second);
    }
}

/**
 * @brief Removes an affiliate from HouseMaster
 * @param affiliate affiliate to remove
 */
void HouseMaster::removeAffiliate(const HouseMasterAffiliate &affiliate) {
    for (auto it = _clientContacts.begin(); it != _clientContacts.end();) {
        if ((*it)->getAffiliate() == affiliate.getAffiliateName()) {
            it = _clientContacts.erase(it);
        } else it++;
    }
    _affiliates.remove(affiliate);
}

/**
 * @brief getter
 * @return a vector with the affiliates from the given location
 */
vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByLocation(const string &location) {
    vector<HouseMasterAffiliate> affiliates_from_location;
    for (auto it = _affiliates.begin(); it != _affiliates.end(); it++) {
        if ((*it).getLocation() == location) {
            affiliates_from_location.push_back(*it);
        }
    }
    return affiliates_from_location;
}

/**
 * @brief getter
 * @return a vector with the affiliates from the given responsible name
 */
vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByResponsible(const string &responsible) {
    vector<HouseMasterAffiliate> affiliates_from_responsible;
    for (auto it = _affiliates.begin(); it != _affiliates.end(); it++) {
        if ((*it).getAdmin().getName() == responsible) {
            affiliates_from_responsible.push_back(*it);
        }
    }
    return affiliates_from_responsible;
}

/**
 * @brief Finds an affiliate by client
 * @param client the client
 * @return the affiliate
 */
HouseMasterAffiliate HouseMaster::findAffiliateByClient(const Client *client) const {
    BSTItrIn<HouseMasterAffiliate> current(_affiliates);
    for (; !current.isAtEnd(); current.advance()) {
        auto affiliate = current.retrieve();
        for (const auto &i : affiliate.getAffiliateClients()) {
            if (i->getId() == client->getId()) {
                std::cout << "Found client in " << affiliate.getAffiliateName() << "\n";
                return affiliate;
            }
        }
    }
    return HouseMasterAffiliate();
}

/**
 * @brief Finds an affiliate by collaborator
 * @param collab the collaborator
 * @return the affiliate
 */
HouseMasterAffiliate HouseMaster::findAffiliateByCollab(const Collaborator *collab) const {
    BSTItrIn<HouseMasterAffiliate> current(_affiliates);
    for (; !current.isAtEnd(); current.advance()) {
        auto affiliate = current.retrieve();
        for (const auto &i : affiliate.getAffiliateCollabs()) {
            if (i->getId() == collab->getId()) {
                return affiliate;
            }
        }
    }
    return HouseMasterAffiliate();
}

/**
 * @brief saves the affiliates' info
 */
void HouseMaster::writeAffiliatesInfo() {
    std::ofstream affiliatesFile("../data/affiliate.txt");
    if (affiliatesFile.is_open()) {
        BSTItrIn<HouseMasterAffiliate> current(_affiliates);
        for (; !current.isAtEnd(); current.advance()) {
            affiliatesFile << current.retrieve().getAffiliateName()
                           << ',' << current.retrieve().getLocation() << ',' << current.retrieve().getAdmin().getName()
                           << '\n';
        }
        affiliatesFile.close();
    } else throw HouseMasterAffiliate::UnableToWriteFile("Unable to write in affiliates file");  // TODO change?
}


// Users Manip
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
    else if (it->second.substr(0, 5) == "admin") return _responsibles[it->second];
    else return nullptr;
}

/**
 * @brief getter
 * @return the clients
 */
std::map<std::string, Client *> HouseMaster::getClients() const {
    return _clients;
}

/**
 * @brief adds a new client to the housemaster
 * @param nif tax id
 * @param name name
 * @param premium is premium
 */
void HouseMaster::addClient(unsigned long nif, const std::string &name, std::string email, bool premium, std::string affiliate) {

    auto it = std::find_if(_clients.begin(), _clients.end(), [&nif](const std::pair<std::string, Client *> &pair) {
        return pair.second->getNif() == nif;
    });
    if (it == _clients.end()) {
        auto client = new Client(nif, name, email, premium, std::move(affiliate));
        _clients.insert({client->getId(), client});
        _clientContacts.insert(client);
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
 * @brief Finds a client by it's email
 * @param email the email
 * @return the client
 */
Client * HouseMaster::findClientByEmail(const string &email) const {
    for (const auto &client : _clientContacts) {
        if (client->getEmail() == email) {
            return client;
        }
    }
    throw HouseMaster::NonexistentClient("Email not found in records");
}

/**
 * @brief Changes client's email
 * @param oldEmail the previous email
 * @param newEmail the new email
 */
void HouseMaster::changeClientEmail(const string &oldEmail, const string &newEmail) {
    Client *toChange = findClientByEmail(oldEmail);
    _clientContacts.erase(findClientByEmail(oldEmail));
    toChange->setEmail(newEmail);
    _clientContacts.insert(toChange);
}

/**
 * @brief getter
 * @return the clients' contacts
 */
clientHT HouseMaster::getContacts() const {
    return _clientContacts;
}

/**
 * @brief Add An std::pair<username, Id> to the _usernames Map, used to save (And possibly login) Collaborators and
 * administrators
 * @param mapElem The element to add
 */
void HouseMaster::addUsernamesMapEntry(std::pair<std::string, std::string> mapElem) {
    _usernameMap.emplace(mapElem);
}

/**
 * @brief getter
 * @return the collaborators
 */
std::map<std::string, Collaborator *> HouseMaster::getCollaborators() const {
    return _collaborators;
}

/**
 * @brief removes a collaborator from housemaster (general)
 * @param id the id of the collaborator to remove
 */
void HouseMaster::removeCollaborator(const std::string &id) {
    auto it = this->_collaborators.find(id);
    auto usernameIt = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                                   [&id](const std::pair<std::string, std::string> &mapped) {
                                       return mapped.second == id;
                                   });
    if (it != _collaborators.end()) {
        if (usernameIt != _usernameMap.end()) {
            _collaborators.erase(it);
            _usernameMap.erase(usernameIt);
        } else {
            throw NonexistentUsername("This username does not exist!");
        }
    } else {
        throw NonexistentCollab("There's no such collab!");
    }
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
void HouseMaster::addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro,
                                  std::vector<Availability> availabilities, float earnings,
                                  Classification score, std::string affiliate) {
    auto collab = new Collaborator(services, name, pro, availabilities, earnings, score, std::move(affiliate));
    _collaborators.insert({collab->getId(), collab});
    _usernameMap.insert({collab->getId(), collab->getId()});
}

/**
 * @brief Function to get the collaborator knowing its id
 * @param collabId
 * @return Collaborator *
 */
Collaborator *HouseMaster::findCollabById(const std::string &collabId) {
    return _collaborators[collabId];
}

/**
 * @brief adds a new admin to the housemaster
 * @param name name of the collaborator
 * @param password the admins' password
 * @param affiliates the affiliates
 */
void HouseMaster::addAdmin(const std::string &name, std::string password, const std::vector<std::string> &affiliates) {
    auto admin = new Admin(name, password, affiliates);
    for (const auto &i : _responsibles) {
        if (i.second->getName() == admin->getName() && i.second->getAffiliates() == admin->getAffiliates()) {
            return;
        }
    }
    _responsibles.insert({admin->getId(), admin});
    _usernameMap.insert({admin->getId(), admin->getId()});
}

/**
 * @brief Manipulates the username map
 * @param string with the user id
 */
void HouseMaster::usernameMapChanger(std::string id, std::string newUsername) {
    std::cout << "id is " << id << "\n";

    auto user = std::find_if(_usernameMap.begin(), _usernameMap.end(),
                             [&id](const std::pair<std::string, std::string> &mapElem) {
                                 std::cout << mapElem.second << "..\n";
                                 return mapElem.second == id;
                             });

    if (user != _usernameMap.end()) {
        auto alreadyInUse = std::find_if_not(_usernameMap.begin(), _usernameMap.end(),
                                             [&newUsername](const std::pair<std::string, std::string> &username) {
                                                 return newUsername != username.first;
                                             });

        if (alreadyInUse == _usernameMap.end()) {
            std::cout << "changing username......\n";
            _usernameMap.erase(user);
            _usernameMap.emplace(std::pair<std::string, std::string>(newUsername, id));
        } else {
            throw UsernameAlreadyInUse("Username in use, choose another!");
        }
    } else {}
}

/**
 * @brief Saves usernameMap
 */
void HouseMaster::writeUsernameMap() {
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

// General
/**
 * @brief getter
 * @return the total earnings
 */
float HouseMaster::getTotalFinances() const {
    float totalFinances{};
    for (auto it = _affiliates.begin(); it != _affiliates.end(); it++) {
        totalFinances += (*it).getEarnings();
    }
    return totalFinances;
}

// Exceptions
/**
 * @brief Exception thrown when trying to change username to another already in use
 * @param error_msg to show
 */
HouseMaster::UsernameAlreadyInUse::UsernameAlreadyInUse(const std::string &error_msg) : std::logic_error(error_msg) {}

/**
 * @brief the exception for nonexistent username
 * @param error_msg to show
 */
HouseMaster::NonexistentUsername::NonexistentUsername(const std::string &error_msg) : out_of_range(error_msg) {}

/**
 * @brief the exception for a nonexistent collaborator
 * @param error_msg to show
 */
HouseMaster::NonexistentCollab::NonexistentCollab(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
* @brief the exception for nonexistent responsible
* @param error_msg to show
*/
HouseMaster::NonexistentResponsible::NonexistentResponsible(const std::string &error_msg) : out_of_range(error_msg) {}


/**
 * @brief the exception for when a client already exists
 * @param error_msg to show
 */
HouseMaster::ExistentClient::ExistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for nonexistent clients
 * @param error_msg to show
 */
HouseMaster::NonexistentClient::NonexistentClient(const std::string &error_msg) : std::out_of_range(error_msg) {}

/**
 * @brief the exception for writing to file failures
 * @param error_msg to show
 */
HouseMaster::UnableToWriteFile::UnableToWriteFile(const std::string &error_msg) : std::ifstream::failure(error_msg) {}







// Individual related code
/**
 * @brief Changes username in username map
 * @param hm, the instance of housemaster
 */
void Individual::changeUsername(HouseMaster &hm, std::string newUsername) const {
    std::cout << "Changing " << this->getId() << " to be " << newUsername << "\n";
    hm.usernameMapChanger(this->getId(), std::move(newUsername));
}

/**
* @brief adds a new classification to a collaborator
* @param hm the instance of housemaster
* @param collabId the collaborator to classify
* @param classification the classification to give the collab
*/
void
Client::classifyCollaborator(HouseMaster *hm, const std::string &collabId, Classification classification) {
    hm->findCollabById(collabId)->addClassification(classification);
}



// HouseMasterAffiliate related code
/**
 * @brief housemaster constructor from files
 * @param collaborators collabs info
 * @param clients clients info
 * @param services services info
 * @param earnings earnings info
 */
HouseMasterAffiliate::HouseMasterAffiliate(HouseMaster *hm, std::ifstream usernames, std::ifstream collaborators,
                                           std::ifstream clients, std::ifstream services, std::ifstream history,
                                           std::ifstream responsibles,
                                           std::string location, const std::string &hmName, float finances)
        : _hm(hm), _name(hmName), _location(std::move(location)), _earnings(finances) {

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

        try {
            if (category == "default") {
                _hm->addAvailableService(name, pro, price, duration);
            } else if (category == "painting") {
                _hm->addAvailablePaintService(name, pro, price, duration);
            }
        }
        catch (const HouseMasterAffiliate::ExistentService &e) {
            //std::cout << e.what() << std::endl;
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
            // availability
            std::string availabilitiesStr{};
            std::string availabilityStr{};
            std::vector<Availability> availabilities;
            std::getline(lss, availabilitiesStr, ',');
            std::stringstream availabilitiesStream(availabilitiesStr);
            while (std::getline(availabilitiesStream, availabilityStr, ' ')) {
                availabilities.emplace_back(Availability(availabilityStr));
            }
            // services
            std::string serviceName{};
            std::vector<std::string> collabServices;

            while (std::getline(lss, serviceName, ',')) {
                collabServices.push_back(serviceName);
            }

            _hm->addCollaborator(collabServices, name, proStr == "yes", availabilities, collabEarnings, score,
                                 affiliateName);
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
            std::string email;
            std::string nifStr{};
            std::string premiumStr{};
            std::getline(lss, name, ',');
            std::getline(lss, email, ',');
            std::getline(lss, nifStr, ',');
            std::getline(lss, premiumStr, ',');

            _hm->addClient(std::stoul(nifStr), name, email, premiumStr == "yes", affiliate_name);
        } else continue;
    }

    // read usernames.txt
    for (std::string line; std::getline(usernames, line);) {
        std::stringstream lss(line);
        std::string username;
        std::string id;
        std::getline(lss, username, ',');
        std::getline(lss, id, ',');
        _hm->addUsernamesMapEntry(std::pair<std::string, std::string>(username, id));
    }


    // read history.txt
    for (std::string line; std::getline(history, line);) {
        if (line.front() == '@') continue;
        else if (line.front() == ' ') break;
        std::stringstream lineStream(line);
        std::string serviceStr{}, clientId{}, collabId{}, start{}, forcePro{}, state{}, cost{}, nrRooms{};
        std::getline(lineStream, serviceStr, ',');
        Service *service = _hm->getAvailableServices().at(serviceStr);
        std::getline(lineStream, clientId, ',');
        std::getline(lineStream, collabId, ',');
        std::getline(lineStream, start, ',');
        std::getline(lineStream, forcePro, ',');
        std::getline(lineStream, state, ',');
        std::getline(lineStream, cost, ',');
        std::getline(lineStream, nrRooms, ',');
        /*auto *intervention = new Intervention(Date(start), service, forcePro == "1", std::stoi(nrRooms),
                                              processState(std::stoi(state)), std::stof(cost),
                                              collabId, clientId);*/
    }

    // read responsibles
    for (std::string line; std::getline(responsibles, line);) {
        std::stringstream lineStream(line);
        // name
        std::string name;
        std::getline(lineStream, name, ',');
        // password
        std::string password;
        std::getline(lineStream, password, ',');
        // affiliates
        std::string affiliate{};
        std::vector<std::string> affiliates;
        while (std::getline(lineStream, affiliate, ',')) {
            affiliates.push_back(affiliate);
        }
        for (const auto &i : affiliates) {
            if (_location == i) {
                _hm->addAdmin(name, password, affiliates);
            }
        }
    }
}

/**
 * @brief housemaster constructor
 * @param location the location
 * @param hmName the affiliate's name
 */
HouseMasterAffiliate::HouseMasterAffiliate(HouseMaster *hm, std::string location, std::string hmName)
        : _hm(hm), _name(std::move(hmName)), _location(std::move(location)){
    _earnings = 0.0;
}

/**
 * @brief Removes collaborator from housemaster, checking for its active interventions
 * @param collId
 */
void HouseMasterAffiliate::removeCollaborator(const std::string &collId) {
    if (getAssociatedActiveInterventions(collId).empty()) {
        _hm->removeCollaborator(collId);
    } else {
        throw AssignedCollab("Collaborator still has incomplete Interventions!");
    }
}
/**
 * @brief Function to get all of the clients of one affiliate
 * @return Vector with clients
 */
std::vector<Client *> HouseMasterAffiliate::getAffiliateClients() const {
    std::vector<Client *> clients;
    for (const auto &pair: _hm->getClients()) {
        if (pair.second->getAffiliate() == getAffiliateName())
            clients.emplace_back(pair.second);
    }
    return clients;
}

/**
 * @brief Function to get all of the collaborators of one affiliate
 * @return Vector with Collaborators
 */
std::vector<Collaborator *> HouseMasterAffiliate::getAffiliateCollabs() const {
    std::vector<Collaborator *> collabs;
    for (const auto &pair: _hm->getCollaborators()) {
        if (pair.second->getAffiliate() == getAffiliateName())
            collabs.emplace_back(pair.second);
    }
    return collabs;
}

/**
 * @brief processes the payment of an intervention
 * @param intervention the intervention
 */
void HouseMasterAffiliate::processTransaction(Intervention *intervention) {
    float hmEarnings;
    hmEarnings = intervention->getCost() - (intervention->getCost() / float(1 + HouseMasterTax));
    _hm->findCollabById(intervention->getCollabId())->calculateEarnings(hmEarnings);
    _earnings += hmEarnings;
}


/**
 * @brief adds a service to the housemaster's catalogue
 * @param name name of the service
 * @param pro requires professional
 * @param basePrice base price
 * @param duration duration
 */
void HouseMaster::addAvailableService(const std::string &name, bool pro, float basePrice,
                                               const Duration &duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw HouseMasterAffiliate::ExistentService("A service with the same name already exists!");
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
void HouseMaster::addAvailablePaintService(const std::string &name, bool pro, float basePrice,
                                                    const Duration &duration) {
    if (_availableServices.find(name) != _availableServices.end())
        throw HouseMasterAffiliate::ExistentService("A service with the same name already exists!");
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
        throw HouseMasterAffiliate::NonexistentService("There's no such service!");
    }
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
    auto services = getAvailableServices();
    auto it = std::find(services.begin(), services.end(), service);
    if (it == services.end()) throw NonexistentService("There's no such service!");
    auto newIntervention = new Intervention(start, _hm->getAvailableServices().at(service), forcePro, nrOfRooms, Active, 0, "",
                                            clientId);
    _interventions.insert(newIntervention);
    return newIntervention;
}

/**
 * @brief saves the clients' info
 */
void HouseMaster::writeClientsInfo() {
    std::ofstream clientsFile("../data/clients.txt");
    if (clientsFile.is_open()) {
        auto clients = _clients;
        for(const auto & client : clients){
            clientsFile << client.second->getAffiliate() << "," << client.second->getName() << ","
            << client.second->getEmail() << "," << client.second->getNif();
            if (client.second->isPremium()) { clientsFile << ",yes"; } else { clientsFile << ",no"; }
            clientsFile << '\n';
        }
        clientsFile.close();
    } else throw UnableToWriteFile("Unable to write in clients' file");
}

/**
 * @brief saves the services' info
 */
void HouseMaster::writeServicesInfo() {
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

const unordered_map<std::string, Service *> &HouseMaster::getAvailableServices() const {
    return _availableServices;
}


/**
 * @brief saves the collaborators' info
 */
void HouseMaster::writeCollabsInfo() {
    std::ofstream collabFile;
    collabFile.open("../data/collabs.txt");
    if (collabFile.is_open()) {
        auto collabs = _collaborators;
        for(const auto& collab : collabs){
            collabFile << collab.second->getAffiliate() << ",";
            collabFile << collab.second->getName();
            if (collab.second->isPro()) { collabFile << ",yes,"; } else { collabFile << ",no,"; }
            collabFile << collab.second->getEarnings() << ',' << collab.second->getScore() << ',';
            for (size_t i = 0; i < collab.second->getAvailability().size(); i++) {
                if (i == collab.second->getAvailability().size() - 1)
                    collabFile << collab.second->getAvailability()[i].getString() << ',';
                else collabFile << collab.second->getAvailability()[i].getString() << " ";
            }
            for (size_t i = 0; i < collab.second->getServices().size(); i++) {
                if (i == collab.second->getServices().size() - 1)
                    collabFile << collab.second->getServices()[i];
                else collabFile << collab.second->getServices()[i] << ",";
            }
            collabFile << '\n';
        }
        collabFile.close();
    } else throw UnableToWriteFile("Unable to write in collabs' file");
}
