#pragma once

#include<string>
#include<vector>
#include"Services.h"


class Individual{
public:
    explicit Individual(std::string name);
    unsigned int getId() const;
    std::string getName();
    bool operator== (const Individual& right);
    static unsigned int _idSeq;

protected:
    std::string _name;
    unsigned int _id;
    std::vector<Intervention* > _associatedInterventions;

};