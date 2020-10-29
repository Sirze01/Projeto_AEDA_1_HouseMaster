#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include<string>
#include<vector>
#include <cmath>
#include"Services.h"


class Individual {
public:
    explicit Individual(std::string name);

    unsigned int getId() const;

    std::string getName();

    bool operator==(const Individual &right);

    const std::vector<Intervention *> &getAssociatedInterventions() const;

protected:
    std::string _name;
    unsigned int _id;
    std::vector<Intervention *> _associatedInterventions;
};


// Collaborator associated code
enum Classification {
    unreliable = -2,
    clumsy = -1,
    newHere = 0,
    getsItDone = 1,
    hardWorker = 2,
    attentive = 3,
    savior = 4
};


class Collaborator : public Individual {
public:
    Collaborator(std::vector<Service *> functions, const std::string &name);

    std::vector<Service *> getServices();

    bool canPreform(Service *service);

    bool isAvailable(date start, date duration);

    int getScore();

    void addClassification(Classification classification);

    Classification getAverageClassification();

    static unsigned int _idSeq;

private:
    std::vector<Classification> _classifications;
    std::vector<Service *> _services;
    Classification _score;
};


class Client : public Individual {
public:
    Client(unsigned int nif, const std::string &name);

    unsigned int getNif();

    int requestIntervention(date appointment, Service type, bool forcePro);

    static unsigned int _idSeq;

    const std::vector<Intervention *> &getRequestedInterventions() const;


private:
    unsigned int _nif;
    std::vector<Intervention *> _requestedInterventions;
};

#endif
