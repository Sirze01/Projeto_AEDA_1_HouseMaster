#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {

    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    Client* c1 = houseMaster.getClients().front();
    date d1{};
    d1.day = 24; d1.hour = 1; d1.minute = 3; d1.month = 10; d1.year = 2020;

    servicesType* service1 = houseMaster.getCollaborators().front()->getFunctions().front();
    servicesType* service2 = houseMaster.getCollaborators().back()->getFunctions().back();

    c1->requestIntervention(d1, *service1, false, 3);
    c1->requestIntervention(d1, *service2, false, 1);
    houseMaster.updateInterventions();

    std::cout << "REGISTERED INTERVENTIONS:\n";

    for (const auto &i : houseMaster.getInterventions()) {
        std::cout << i->getType().type << "\n";
    }
    return 0;
}
