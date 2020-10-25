#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include<string>
#include<vector>
#include"Services.h"


class Individual{
public:
    explicit Individual(std::string name);
    unsigned int getId() const;
    std::string getName();
    bool operator== (const Individual& right);

protected:
    std::string _name;
    unsigned int _id;
    std::vector<Intervention* > _associatedInterventions;

};


// Collaborator associated code
enum Classification {
    unreliable,
    clumsy,
    newHere,
    getsItDone,
    hardWorker,
    attentive,
    savior
};


class Collaborator: public Individual
{
public:
    Collaborator(const std::vector<Service*>& functions, const std::string& name);
    std::vector<Service*> getFunctions();
    int getScore();
    int getAvailability();
    static unsigned int _idSeq;

private:
    std::vector<Service*> functions;
    Classification score;
    bool availability;
};




class Client: public Individual
{
public:
    Client(unsigned int nif, const std::string &name);
    unsigned int getNif();
    int requestIntervention(date appointment, Service type, bool forcePro);
    static unsigned int _idSeq;

    const std::vector<Intervention *> &getRequestedInterventions() const;

private:
    unsigned int _nif;
    std::vector<Intervention* > _requestedInterventions;
};

#endif
