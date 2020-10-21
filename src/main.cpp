#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {

    // testing houseMaster constructor
    /*
    std::cout << "Hello, World!" << std::endl;

    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

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

    return 0;
}
