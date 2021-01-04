#include "HouseMasterAffiliate.h"

#include <utility>

// unsigned int HouseMasterAffiliate::_idSeqAffiliate = 0; ??

/**
 * @brief compares two collaborators' score
 * @param a first collaborator
 * @param b second collaborator
 * @return true first has better score false otherwise
 */
bool scoreComparer(Collaborator * a, Collaborator * b) {
    return a->getScore() > b->getScore();
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
 * @brief requests and processes an intervention according to the client's requirements
 * @param hm the instance of housemaster
 * @param date the desired date
 * @param service the service
 * @param forcePro
 * @param nrOfRooms the number of rooms
 */
void Client::requestIntervention(HouseMasterAffiliate &hm, const Date &date, const std::string &service, bool forcePro,
                                 unsigned int nrOfRooms) const {
    hm.assignCollaborator(hm.addIntervention(date, service, forcePro, this->getId(), nrOfRooms));
}

/**
 * @brief cancels an intervention
 * @param intervention to cancel
 */
void Client::cancelIntervention(Intervention *intervention) {
    HouseMasterAffiliate::changeInterventionState(intervention, Canceled);
}




/**
 * @brief checks if a collaborator is available for a service
 * @param hm the instance of housemaster
 * @param collabId the id of the collaborator
 * @param start of intervention
 * @param duration of intervention
 * @return true if available otherwise false
 */
bool Collaborator::isAvailable(HouseMasterAffiliate &hm, const std::string &collabId, Date start, Duration duration) const {
    std::vector<Availability> availability = this->getAvailability();
    auto fStart = std::find_if(availability.begin(), availability.end(), [&start](const Availability& availability1) {
        return start.getDate().tm_wday == availability1.getWeekday();
    });
    if(fStart == availability.end())
        return false;

    auto fEnd = std::find_if(availability.begin(), availability.end(), [&start, &duration](const Availability& availability1){
        Date temp;
        temp = start + duration;
        return temp.getDate().tm_wday == availability1.getWeekday();
    });
    if(fEnd == availability.end())
        return false;

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
// Contructor and Destructor
/**
 * @brief housemaster constructor
 */
HouseMasterAffiliate::HouseMasterAffiliate(HouseMaster* hm): _hm(hm), _interventions(), _earnings(), _responsible() {

}




// Users Manip


// Services and Interventions Manip
/**
 * @brief getter
 * @return the interventions
 */
std::unordered_set<Intervention *> &HouseMasterAffiliate::getInterventions() {
    return _interventions;
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
std::vector<Collaborator *> HouseMasterAffiliate::sortCollaboratorsByScore() const{
    std::vector<Collaborator *> temp;
    for (const auto &elem : getAffiliateCollabs()) {
        temp.emplace_back(elem);
    }
    std::sort(temp.begin(), temp.end(), [](const Collaborator* c1, const Collaborator* c2) {
        return c1->getScore() > c2->getScore();
    });
    return temp;
}


/**
 * @brief assigns a collaborator to an intervention
 * @param intervention the intervention
 * @param orderedCollabs the collaborators
 */
void HouseMasterAffiliate::assignCollaborator(Intervention *intervention) {
    HouseMasterAffiliate *hm = this;

    for(const auto& collab : getAffiliateCollabs()){
        if (collab->canDo(*hm, collab->getId(), intervention))
           _collaborators_queue.emplace(std::pair<Intervention*, Collaborator*>(intervention, collab));
    }

    if (!_collaborators_queue.empty())
        intervention->setCollabId(_collaborators_queue.top().second->getId());
    else throw UnavailableAppointment("No collaborators available to the desired date!");
}



/**
 * @brief saves the collaborators' info
 */
void HouseMasterAffiliate::writeCollabsInfo() {
    std::ofstream collabFile;
    collabFile.open("../data/collabs.txt");
    if (collabFile.is_open()) {
        std::vector<Collaborator*> collabs = getAffiliateCollabs();
        for(const auto& collab : collabs){
            collabFile << collab->getAffiliate() << ',';
            collabFile << collab->getName();
            if (collab->isPro()) { collabFile << ",yes,"; } else { collabFile << ",no,"; }
            collabFile << collab->getEarnings() << ',' << collab->getScore() << ',';
            for (size_t i = 0; i < collab->getAvailability().size(); i++) {
                if (i == collab->getAvailability().size() - 1)
                    collabFile << collab->getAvailability()[i].getString() << ' ';
                else collabFile << collab->getAvailability()[i].getString() << ",";
            }
            for (size_t i = 0; i < collab->getServices().size(); i++) {
                if (i == collab->getServices().size() - 1)
                    collabFile << collab->getServices()[i];
                else collabFile << collab->getServices()[i] << ",";
            }
            collabFile << '\n';
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
        auto clients = getAffiliateClients();

        for(const auto & client : clients){
            clientsFile << client->getName() << "," << client->getNif();
            if (client->isPremium()) { clientsFile << ",yes"; } else { clientsFile << ",no"; }
            clientsFile << '\n';
        }
        clientsFile.close();
    } else throw UnableToWriteFile("Unable to write in clients' file");
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
 * @brief the exception for a nonexistent service
 * @param error_msg to show
 */
HouseMasterAffiliate::NonexistentService::NonexistentService(const std::string &error_msg) : std::out_of_range(
        error_msg) {}


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
std::string HouseMasterAffiliate::getAdmin_str() const {
    return _responsible.getName();
}

/**
 * @brief exception for an impossible appointment
 * @param error_msg to show
 */
HouseMasterAffiliate::UnavailableAppointment::UnavailableAppointment(const std::string &error_msg) : std::logic_error(
        error_msg) {}

/**
 * @brief the exception for when trying to fire a collaborator with active interventions
 * @param error_msg to show
 */
HouseMasterAffiliate::AssignedCollab::AssignedCollab(const std::string &error_msg) : std::logic_error(error_msg) {}

/**
 * @brief the exception for writing to file failures
 * @param error_msg to show
 */
HouseMasterAffiliate::UnableToWriteFile::UnableToWriteFile(const std::string &error_msg) : std::ifstream::failure(
        error_msg) {}

/**
 * @brief The < operator of the HouseMasterAffiliate Class, needed for the BST
 * @param hma Another affiliate
 * @return Returns true if this Hma is "smaller"(meets the chosen criteria) than hma, false otherwise
 */
bool HouseMasterAffiliate::operator<(const HouseMasterAffiliate &hma) const {
    if (_responsible.getName() == hma.getAdmin().getName()) {
        if (getAffiliateClients().size() == hma.getAffiliateClients().size()) {
            return (_location < hma.getLocation());
        } else return (getAffiliateClients().size() < hma.getAffiliateClients().size());
    } else return (_responsible.getName() < hma.getAdmin().getName());
}

/**
 * @brief Sets the affilliate admin
 * @param admin
 */
void HouseMasterAffiliate::setAdmin(const Admin& admin) {
    _responsible = admin;
}

/**
 * @brief Sets the parent Housemaster
 * @param hm
 */
void HouseMasterAffiliate::setHousemaster(HouseMaster *hm) {
    _hm = hm;
}

