#include "HouseMaster.h"

/**
 * @brief getter
 * @return the affiliates
 */
BST<HouseMasterAffiliate > &HouseMaster::getAffiliates() {
    return _affiliates;
}

/**
 * @brief housemaster constructor
 */
HouseMaster::HouseMaster() : _affiliates(HouseMasterAffiliate()) {

}

/**
 * @brief adds a new affiliate to the housemaster
 * @param affiliate the affiliate
 */
void HouseMaster::registerAffiliate(const HouseMasterAffiliate& affiliate) {
    _affiliates.insert(affiliate);
    for (const auto &client : affiliate.getClients()) {
        _clientContacts.insert(client.second);
    }
}


/**
 * @brief getter
 * @return the clients' contacts
 */
clientHT HouseMaster::getContacts() const {
    return _clientContacts;
}


/**
 * @brief housemaster constructor from files
 * @param affiliates affiliates' info
 */
HouseMaster::HouseMaster(std::ifstream affiliates) : _affiliates(HouseMasterAffiliate()) {
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
        auto h1 = HouseMasterAffiliate(std::ifstream("../data/usernames.txt"),
                                           std::ifstream("../data/collabs.txt"),
                                           std::ifstream("../data/clients.txt"),
                                           std::ifstream("../data/services.txt"),
                                           std::ifstream("../data/history.txt"),
                                            location, responsible, name, earnings);
        _locations.insert(location);
        _responsibles.insert(responsible);
        registerAffiliate(h1);
    }
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
            << ',' << current.retrieve().getLocation() << ',' << current.retrieve().getAdmin().getName() << '\n';
        }
            affiliatesFile.close();
    } else throw HouseMasterAffiliate::UnableToWriteFile("Unable to write in affiliates file");  // TODO change?
}

/**
 * @brief getter
 * @return a vector with the affiliates from the given location
 */
vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByLocation(const string& location)
{
    vector<HouseMasterAffiliate> affiliates_from_location;
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        if((*it).getLocation() == location)
        {
            affiliates_from_location.push_back(*it);
        }
    }
    return affiliates_from_location;
}

/**
 * @brief getter
 * @return a vector with the affiliates from the given responsible name
 */
vector<HouseMasterAffiliate> HouseMaster::getAffiliatesByResponsible(const string& responsible)
{
    vector<HouseMasterAffiliate> affiliates_from_responsible;
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        if((*it).getAdmin().getName() == responsible)
        {
            affiliates_from_responsible.push_back(*it);
        }
    }
    return affiliates_from_responsible;
}

/**
 * @brief getter
 * @return the total earnings
 */
float HouseMaster::getTotalFinances()
{
    float totalFinances{};
    for(auto it = _affiliates.begin(); it != _affiliates.end(); it++)
    {
        totalFinances += (*it).getEarnings();
    }
    return totalFinances;
}

void HouseMaster::removeAffiliate(const HouseMasterAffiliate &affiliate) {
    for (auto it = _clientContacts.begin(); it != _clientContacts.end(); ) {
        if ((*it)->getAffiliate() == affiliate.getAffiliateName()) {
            it = _clientContacts.erase(it);
        } else it++;
    }
    _affiliates.remove(affiliate);
}
