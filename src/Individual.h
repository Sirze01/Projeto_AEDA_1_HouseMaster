#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include <string>
#include <vector>
#include <cmath>
#include <numeric>
#include "Services.h"

class Individual {
public:

    Individual() = default;

    explicit Individual(std::string name);

    unsigned int getId() const;

    std::string getName();
  
    bool operator== (const Individual& ind2) const;

    const std::vector<Intervention *> &getAssociatedInterventions() const;

    virtual std::string getInfo() const;

    std::string getUniqueName() const;


protected:
    std::string _displayName;
    std::string _uniqueName;
    unsigned int _id{};
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
    Collaborator(std::vector<Service *> functions, const std::string &name, bool pro);

    std::vector<Service *> getServices();

    bool isPro() const;

    bool canPreform(Service *service);

    bool isAvailable(date start, date duration);

    bool hasQualificationToPreform(Intervention *intervention) const;

    bool canDo(Intervention *intervention);

    int getScore();

    void addClassification(Classification classification);

    void updateScore();

    static unsigned int _idSeq;

    std::string getInfo() const override;

private:
    std::vector<Classification> _classifications;
    std::vector<Service *> _services;
    Classification _score;
    bool _pro;
};


// Client code
class Client : public Individual {
public:
    Client(unsigned int nif, const std::string &name, bool premium);

    unsigned int getNif();

    bool isPremium() const;

    int requestIntervention(date appointment, Service type);

    static unsigned int _idSeq;

    const std::vector<Intervention *> &getRequestedInterventions() const;

    std::string getInfo() const override;


private:
    unsigned int _nif;
    std::vector<Intervention *> _requestedInterventions;
    bool _premium;
};

#endif
