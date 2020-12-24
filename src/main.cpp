#include <iostream>

#include "Interface.h"
#include "HouseMaster.h"


int main() {

    HouseMaster hm(std::ifstream("../data/affiliates.txt"));

    for (const auto & i : hm.getContacts()) {
        std::cout << i->getAffiliate() <<  " " << i->getName() << " " << i->getEmail() << "\n";
    }
    /*
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
    }
    return 0;
     */
}
