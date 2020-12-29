#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H


#include <iostream>
#include "bst.h"
#include "HouseMasterAffiliate.h"


struct ClientHash {
    int operator()(const Client *client) const {
        return std::hash<std::string>()(client->getEmail());
    }
    bool operator()(const Client *c1, const Client *c2) const {
        return c1->getEmail() == c2->getEmail();
    }
};

typedef std::unordered_set<Client*, ClientHash, ClientHash> clientHT;

/**
 * @brief This class keeps the various housemaster affiliates
 */
class HouseMaster {
public:

    HouseMaster();

    explicit HouseMaster(std::ifstream affiliates);

    ~HouseMaster() = default;

    BST<HouseMasterAffiliate >& getAffiliates();

    void registerAffiliate(const HouseMasterAffiliate& affiliate);

    void removeAffiliate(const HouseMasterAffiliate& affiliate);

    std::set<std::string> getLocations() {return _locations;}



    clientHT getContacts() const;

    void writeAffiliatesInfo();

    vector<HouseMasterAffiliate> getAffiliatesByLocation(const string& location);

    vector<HouseMasterAffiliate> getAffiliatesByResponsible(const string& /*(ou outra coisa nsei)*/ responsible);

    float getTotalFinances();

private:
    std::set<std::string> _locations;
    BST<HouseMasterAffiliate> _affiliates;
    clientHT _clientContacts;
};

#endif //SRC_HOUSEMASTER_H
