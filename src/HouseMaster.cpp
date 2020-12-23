#include "HouseMaster.h"

/**
 * @brief getter
 * @return the affiliates
 */
BST<HouseMasterAffiliate *> &HouseMaster::getAffiliates() {
    return _affiliates;
}

HouseMaster::HouseMaster() : _affiliates(new HouseMasterAffiliate()) {

}

void HouseMaster::registerAffiliate(HouseMasterAffiliate* affiliate) {
    _affiliates.insert(affiliate);
    for (const auto &client : affiliate->getClients()) {
        _clientContacts.insert(client.second);
    }
}

clientHT HouseMaster::getContacts() const {
    return _clientContacts;
}

void HouseMaster::registerClient(Client *client) {
    _clientContacts.insert(client);
}
