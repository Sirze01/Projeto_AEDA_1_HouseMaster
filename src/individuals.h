#pragma once

#include <string>
#include <vector>
#include "service.h"

class individual{
public:
    explicit individual(std::string name);
    unsigned int getId();
    std::string getName();
    bool operator== (const individual& right);

protected:
    std::string _name;
    unsigned int _id;
    std::vector<interventions>* _associatedInterventions;

};