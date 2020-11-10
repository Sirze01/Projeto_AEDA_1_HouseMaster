#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include <string>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <numeric>
#include "Services.h"

class HouseMaster;

class Individual {
public:

    Individual();

    explicit Individual(std::string name);

    virtual std::string getId() const = 0;

    std::string getName();

    //void markAsComplete


protected:
    std::string _displayName;
    unsigned int _id;
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
    Collaborator(std::vector<std::string> functions, const std::string &name, bool pro);

    std::vector<std::string> getServices();

    bool isPro() const;

    bool canPreform(const std::string& service);

    bool isAvailable(date start, date duration);

    bool hasQualificationToPreform(Intervention *intervention) const;

    bool canDo(Intervention *intervention);

    int getScore();

    void addClassification(Classification classification);

    void updateScore();

    static unsigned int _idSeqCol;

    std::string getId() const override;

    bool operator== (const Collaborator& ind2) const;

    bool operator< (const Collaborator& col2) const;


private:

    std::unordered_set<Intervention*> _associatedInterventions;
    std::vector<Classification> _classifications;
    std::vector<std::string> _services;
    Classification _score;
    bool _pro;
};


// Client code
class Client : public Individual {
public:
    static unsigned int _idSeqClt;

    Client(unsigned int nif, const std::string &name, bool premium);

    unsigned int getNif();

    bool isPremium() const;

    std::string getId() const override;

    void requestIntervention(HouseMaster& hm, const date &_date, const std::string& type,const std::string &clientId, bool forcePro=false);

    // void cancelIntervention

    bool operator== (const Client& ind2) const;

private:
    unsigned int _nif;
    bool _premium;
};

#endif
