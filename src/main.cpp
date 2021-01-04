#include <iostream>

#include "HouseMaster.h"
#include "HousemasterInterface.h"


int main() {

    HouseMaster h1(std::ifstream("../data/affiliates.txt"));
    BSTItrIn<HouseMasterAffiliate> current(h1.getAffiliates());
    HousemasterInterface i = HousemasterInterface(&h1);
    bool running = true;
    while (running) {
        i.firstInterface(running);
    }
    h1 = i.getHousemasterState();
    try {

        h1.writeUsernameMap();
        h1.writeClientsInfo();
        h1.writeServicesInfo();
        h1.writeCollabsInfo();
    }
    catch (const HouseMasterAffiliate::UnableToWriteFile &e) {
        std::cout << e.what();
    }
    return 0;
}
