#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {
    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    Client* c1 = houseMaster.getClients().front();
    date d1{};
    d1.day = 24; d1.hour = 1; d1.minute = 3; d1.month = 10; d1.year = 2020;

    Service* service1 = houseMaster.getCollaborators().front()->getFunctions().front();
    Service* service2 = houseMaster.getCollaborators().back()->getFunctions().back();

    c1->requestIntervention(d1, *service1, false);
    c1->requestIntervention(d1, *service2, false);
    houseMaster.updateInterventions();

    for(const auto & service: houseMaster.getAvailableServices()){
        std::cout << "\t" << service->name << "\n";
    }

    std::cout << "REGISTERED INTERVENTIONS:\n";



    for (const auto &i : houseMaster.getInterventions()) {
        std::cout << i->getService().name << "\n";
    }
    return 0;
}
