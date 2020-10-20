#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {
    std::cout << "Hello, World!" << std::endl;

    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    std::cout << "Clients:\n";
    for (const auto &i : houseMaster.getClients()) {
        std::cout << i->getId() << ": " << i->getName() << "\n";
    }

    // temos que impedir pessoas com nome vazio ok

    return 0;
}
