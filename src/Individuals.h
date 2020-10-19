#pragma once

#include <string>
#include <vector>
#include "Service.h"


class Individual{
public:
    explicit Individual(std::string name);
    unsigned int getId();
    std::string getName();
    bool operator== (const Individual& right);
    static void idSeqSetter();

protected:
    std::string _name;
    static unsigned int _idSeq;
    unsigned int _id;
    std::vector<Interventions*> _associatedInterventions;

};