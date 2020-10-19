#pragma once

#include <string>
#include <vector>
#include "service.h"

class Individual{
public:
    explicit Individual(std::string name);
    unsigned int getId();
    std::string getName();
    bool operator== (const Individual& right);

protected:
    std::string _name;
    unsigned int _id;
    std::vector<Interventions>* _associatedInterventions;

};