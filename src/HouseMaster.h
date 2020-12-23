#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H


#include <iostream>
#include "bst.h"
#include "HouseMasterAffiliate.h"


/**
 * @brief This class keeps the various housemaster affiliates
 */
class HouseMaster {
public:

    HouseMaster();

    explicit HouseMaster(const BST<HouseMasterAffiliate *>& affiliates);

    ~HouseMaster() = default;

    BST<HouseMasterAffiliate *> &getAffiliates();

private:
    BST<HouseMasterAffiliate *> _affiliates;
};

#endif //SRC_HOUSEMASTER_H
