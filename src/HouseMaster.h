
#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H

#include <vector>
#include <fstream>
#include "individuals.h"

class HouseMaster {
public:
    HouseMaster() = default;
    HouseMaster(std::ifstream colaborators, std::ifstream clients);
private:
    std::vector<individual*> collaborators;
    std::vector<individual*> clients;
};


#endif //SRC_HOUSEMASTER_H
