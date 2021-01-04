
#ifndef PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
#define PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H

#include "HouseMaster.h"
#include "Interface.h"

class HousemasterInterface {
public:
    HousemasterInterface() = default;
    explicit HousemasterInterface(HouseMaster *housemaster);
    void housemasterOperations(bool &running);
    void firstInterface(bool &running);
    void adminLogin();
    void responsibleLogin(const std::string& responsibleId);
    void collabLogin();
    void clientLogin();
    void readNewAffiliateData();
    string selectLocation(bool &running);
    void readNewClientData();
    HouseMaster getHousemasterState() const {return *_houseMaster;}
private:
    HouseMasterAffiliate selectAffiliate(bool &running);
    HouseMasterAffiliate selectResponsibleAffiliate(bool &running);
    void showAffiliateInterface(HouseMasterAffiliate &affiliate);
    HouseMasterAffiliate _currentAffiliate;
    HouseMaster* _houseMaster;
    Individual* _user{};
    string selectResponsible(bool &running);
    void show(const HouseMasterAffiliate &affiliate);
    void showTotalFinances(const HouseMaster &hm);
};


#endif //PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERINTERFACE_H
