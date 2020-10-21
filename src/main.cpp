#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {

    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    // testing houseMaster constructor
    /*
    std::cout << "Hello, World!" << std::endl;


    std::cout << "Collaborators:\n";

    for (const auto& collab : houseMaster.getCollaborators()) {
        std::cout << "Name: " << collab->getName() << "\n";
        std::cout << "Services: ";
        for (const auto& service : collab->getFunctions()) {
            std::cout << service->type << ", ";
        }
        std::cout << "\n";
    }

    std::cout << "Clients:\n";
    for (const auto& client : houseMaster.getClients()) {
        std::cout << client->getId() << ": " << client->getName() << " NIF: " << client->getNif() << "\n";
    }
     */

    // temos que impedir pessoas com nome vazio ok

    auto s1 = new servicesType;
    s1->type = "cortar a relva", s1->pro = false;
    houseMaster.addAvailableService(s1);

    auto s2 = new servicesType;
    s2->type = "passear a máquina de lavar loiça pela casa", s1->pro = true;
    houseMaster.addAvailableService(s2);

    std::cout << "Found " << houseMaster.getAvailableServices().size() << " services!\n";
    for (const auto &i : houseMaster.getAvailableServices()) {
        std::cout << i->type << "\n";
    }

    std::cout << "Removing first..\n";
    houseMaster.removeAvailableService(s2);
    std::cout << "Found " << houseMaster.getAvailableServices().size() << " services!\n";
    for (const auto &i : houseMaster.getAvailableServices()) {
        std::cout << i->type << "\n";
    }


    return 0;
}
