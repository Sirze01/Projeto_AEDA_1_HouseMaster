#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include <string>
#include <unordered_set>
#include <vector>
#include <cmath>
#include <numeric>
#include <utility>
#include <algorithm>
#include <sstream>
#include "Services.h"

class HouseMasterAffiliate;

class Individual {
public:

    Individual();

    virtual ~Individual() = default;

    explicit Individual(std::string name);

    virtual std::string getId() const = 0;

    void changeUsername(HouseMasterAffiliate &hm, std::string newUsername) const;

    std::unordered_set<Intervention *> getAssociatedInterventions(HouseMasterAffiliate &hm) const;

    std::unordered_set<Intervention *> getAssociatedActiveInterventions(HouseMasterAffiliate &hm) const;

    std::string getName() const;


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
    hardWorking = 2,
    attentive = 3,
    savior = 4
};


class Collaborator : public Individual {
public:
    Collaborator(std::vector<std::string> services, const std::string &name, bool pro, float earnings = 0,
                 Classification score = newHere, std::string affiliate = "");

    ~Collaborator() override = default;

    std::vector<std::string> getServices();

    bool isPro() const;

    bool canPreform(const std::string &service);

    static bool isAvailable(HouseMasterAffiliate &hm, const std::string &collabId, Date start, Duration duration);

    bool hasQualificationToPreform(Intervention *intervention) const;

    bool canDo(HouseMasterAffiliate &hm, const std::string &collabId, Intervention *intervention);

    int getScore() const;

    std::string getAffiliate() const;

    void addClassification(Classification classification);

    void addService(Service *service);

    static void markInterventionAsComplete(Intervention *intervention);

    void updateScore();

    void calculateEarnings(float &hmEarnings);

    float getEarnings() const;

    float commissionByScore();

    static unsigned int _idSeqCol;

    std::string getId() const override;

    bool operator==(const Collaborator &ind2) const;

    bool operator<(const Collaborator &col2) const;

    class AlreadyKnows;

    class ServiceRequiresPro;

private:

    std::vector<Classification> _classifications;
    std::vector<std::string> _services;
    Classification _score;
    bool _pro;
    float _earnings;
    std::string _affiliate;
};

class Collaborator::AlreadyKnows: public std::logic_error
{
public:
    explicit AlreadyKnows(const std::string &error_msg);
};

class Collaborator::ServiceRequiresPro: public std::logic_error
{
public:
    explicit ServiceRequiresPro(const std::string &error_msg);
};


// Client code
class Client : public Individual {
public:
    static unsigned int _idSeqClt;

    Client(unsigned int nif, const std::string &name, bool premium);

    ~Client() override = default;

    unsigned int getNif();

    bool isPremium() const;

    std::string getId() const override;

    void requestIntervention(HouseMasterAffiliate &hm, const Date &date, const std::string &service, bool forcePro,
                             unsigned int nrOfRooms = 0) const;

    static void cancelIntervention(Intervention *intervention);

    static void classifyCollaborator(HouseMasterAffiliate &hm, const std::string &collabId, Classification classification);

    bool operator==(const Client &ind2) const;

private:
    unsigned int _nif;
    bool _premium;
};



// Admin code
class Admin : public Individual {
public:
    Admin(const std::string &name, const std::string &affiliate);
    ~Admin() override = default;
    std::string getAffiliate() const;
    static unsigned int _idSeqAdmins;
    std::string getId() const override;

private:
    std::string _affiliate;
};


#endif
