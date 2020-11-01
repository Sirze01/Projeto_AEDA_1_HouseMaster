#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"


int main() {

    HouseMaster h1(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    std::vector<Collaborator *> collabs = h1.getCollaborators();
    std::vector<Service *> services = h1.getAvailableServices();
    std::vector<Client *> clients = h1.getClients();

    Service* s1 = services.front();

    date start(1, 11, 2020, 01, 15, 0);
    date start2(31, 10, 2020, 23, 55, 0);
    date duration(0, 0, 0, 2, 30, 0);

    s1->duration = duration;

    auto i1 = new Intervention(start, *s1, false);
    auto i2 = new Intervention(start2, *s1, false);

    std::cout << i1->conflictsWith(start2, start2 + duration);


    return 0;
}
