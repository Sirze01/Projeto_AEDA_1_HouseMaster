#include <iostream>

#include "Interface.h"



int main() {

    HouseMaster h1(std::ifstream("../data/usernames.txt"),
                   std::ifstream("../data/collabs.txt"),
                   std::ifstream("../data/clients.txt"),
                   std::ifstream("../data/services.txt"),
                   std::ifstream("../data/history.txt"));
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
    catch (const HouseMaster::UnableToWriteFile &e)
    {
        std::cout << e.what();
    }
    return 0;
}
