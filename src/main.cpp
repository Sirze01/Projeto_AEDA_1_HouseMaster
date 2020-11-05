#include <iostream>

#include "Interface.h"



int main() {

    HouseMaster h1(std::ifstream("../../data/collabs.txt"), std::ifstream("../../data/clients.txt"), std::ifstream("../../data/services.txt"));
    Interface i1(h1);
    bool running = true;
    while (running) {
        i1.selectRole(running);
    }

    return 0;
}
