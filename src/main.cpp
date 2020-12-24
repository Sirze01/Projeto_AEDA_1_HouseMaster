#include <iostream>

#include "Interface.h"
#include "HouseMaster.h"


int main() {

    auto h1 = new HouseMasterAffiliate(std::ifstream("../data/usernames.txt"),
                   std::ifstream("../data/collabs.txt"),
                   std::ifstream("../data/clients.txt"),
                   std::ifstream("../data/services.txt"),
                   std::ifstream("../data/finances.txt"),
                   std::ifstream("../data/history.txt"),
                   "Lamego", "Gajo");
    HouseMaster hm{};
    hm.registerAffiliate(h1);
    for (const auto & i : hm.getContacts()) {
        std::cout << i->getName() << " " << i->getEmail() << "\n";
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
