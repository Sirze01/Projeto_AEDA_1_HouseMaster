
#ifndef PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
#define PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H

#include "HouseMaster.h"
#include "Interface.h"

class HousemasterInterface {
public:
    HousemasterInterface() = default;
    explicit HousemasterInterface(const HouseMaster &housemaster);
    void runAffiliateInterface(bool &running);
    void showInterface(bool &running);
    void firstInterface(bool &running);
    //void collaboratorOperations(bool &running);
    void adminLogin();
    void responsibleLogin(const HouseMasterAffiliate &hma);
    void collabLogin();
    void clientLogin();
private:
    HouseMasterAffiliate selectAffiliate(bool &running);
    void showAffiliateInterface(HouseMasterAffiliate &affiliate);
    HouseMasterAffiliate _currentAffiliate;
    HouseMaster _houseMaster;
    HouseMasterAffiliate _houseMasterAffiliate;
    Individual* _user{};
    string selectLocation(bool &running);
    string selectResponsible(bool &running);
    void show(const HouseMasterAffiliate &affiliate);
    void showTotalFinances(const HouseMaster &hm);
};


#endif //PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
