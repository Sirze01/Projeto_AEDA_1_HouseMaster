#include "HouseMaster.h"

/**
 * @brief getter
 * @return the affiliates
 */
BST<HouseMasterAffiliate *> &HouseMaster::getAffiliates() {
    return _affiliates;
}

HouseMaster::HouseMaster(const BST<HouseMasterAffiliate *>& affiliates) : _affiliates(affiliates) {
}

HouseMaster::HouseMaster() : _affiliates(new HouseMasterAffiliate()) {

}
