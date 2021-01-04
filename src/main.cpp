#include <iostream>

#include "HouseMaster.h"
#include "HousemasterInterface.h"


int main() {

    HouseMaster hm(std::ifstream("../data/affiliates.txt"));
    BSTItrIn<HouseMasterAffiliate> current(hm.getAffiliates());
    HousemasterInterface i = HousemasterInterface(hm);
    bool running = true;
    while (running) {
        //i.showInterface(running);
        //i.runAffiliateInterface(running);
        i.firstInterface(running);
    }
    /*
    HouseMasterAffiliate h1 = hm.getAffiliates().findMin();
    Interface i1(h1);
    bool running = true;
    while (running) {
        i1.selectRole(running);
    }
    h1 = i1.getHouseMasterState();
    try
    {
        h1.writeUsernameMap();
        h1.writeClientsInfo();
        h1.writeServicesInfo();
        h1.writeCollabsInfo();
        h1.writeInterventionsInfo();
        h1.writeFinancialInfo();
    }
    catch (const HouseMasterAffiliate::UnableToWriteFile &e)
    {
        std::cout << e.what();
    }*/
    return 0;
}
