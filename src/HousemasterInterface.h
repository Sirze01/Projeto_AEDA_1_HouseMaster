//
// Created by ritam on 27/12/20.
//

#ifndef PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
#define PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H

#include "HouseMaster.h"
#include "Interface.h"

class HousemasterInterface {
public:
    HousemasterInterface() = default;
    explicit HousemasterInterface(const HouseMaster &housemaster);
    void runAffiliateInterface(bool &running);
private:
    HouseMasterAffiliate selectAffiliate(bool &running);
    void showAffiliateInterface(HouseMasterAffiliate &affiliate);
    HouseMasterAffiliate _currentAffiliate;
    HouseMaster _houseMaster;
};


#endif //PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
