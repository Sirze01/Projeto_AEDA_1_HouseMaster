#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"



int main() {

    HouseMaster h1(std::ifstream("../../data/collabs.txt"), std::ifstream("../../data/clients.txt"), std::ifstream("../../data/services.txt"));

    return 0;
}
