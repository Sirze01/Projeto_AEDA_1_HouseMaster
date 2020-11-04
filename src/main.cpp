#include <iostream>

#include "Interface.h"



int main() {

    HouseMaster h1(std::ifstream("../../data/collabs.txt"), std::ifstream("../../data/clients.txt"), std::ifstream("../../data/services.txt"));
    Interface i1(h1);

    i1.selectRole();

    return 0;
}
