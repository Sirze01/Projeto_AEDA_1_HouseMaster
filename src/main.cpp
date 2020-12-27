#include <iostream>

#include "Interface.h"
#include "HouseMaster.h"


int main() {

    HouseMaster hm(std::ifstream("../data/affiliates.txt"));
    BSTItrIn<HouseMasterAffiliate> current(hm.getAffiliates());
    for (; !current.isAtEnd(); current.advance()) {
        std::cout << current.retrieve().getAffiliateName() << ".." << current.retrieve().getAdmin().getName()
        << " " << current.retrieve().getLocation() << " " << current.retrieve().getClients().size()<< "\n";
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
