#pragma once

#include <string>
#include <vector>
#include "service.h"

class individual{
public:
    explicit individual(std::string name);
    unsigned int getId();
    std::string getName();
    std::vector<interventions>::iterator findIndividual(std::string name, unsigned int id=0);

protected:
    std::string _name;
    unsigned int _id;
    std::vector<interventions>* _associatedInterventions;

};