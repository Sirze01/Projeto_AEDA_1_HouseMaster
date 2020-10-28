#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {
    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    Client* c1 = houseMaster.getClients().front();
    Client* c2 = houseMaster.getClients().back();
    date d1{};
    d1.day = 24; d1.hour = 1; d1.minute = 3; d1.month = 10; d1.year = 2020;

    Service* service1 = houseMaster.getCollaborators().front()->getServices().front();
    Service* service2 = houseMaster.getCollaborators().back()->getServices().back();

    c1->requestIntervention(d1, *service1, false);
    c1->requestIntervention(d1, *service2, false);
    c2->requestIntervention(d1, *service2, false);
    houseMaster.updateInterventions();

    for(const auto & service: houseMaster.getAvailableServices()){
        std::cout << "\t" << service->name << "\n";
    }

    std::cout << "REGISTERED INTERVENTIONS:\n";



    for (const auto &i : houseMaster.getInterventions()) {
        std::cout << i.first->getName() << " requested " << i.second->getService().name << "\n";
        for (const auto &collab : houseMaster.getCollaborators()) {
            auto *service = new Service(i.second->getService());
            std::cout << collab->getName() << " Can Preform ? " << collab->canPreform(service) << "\n";
        }
        std::cout << "\n";
    }
    return 0;
}
