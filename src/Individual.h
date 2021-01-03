#ifndef SRC_INDIVIDUAL_H
#define SRC_INDIVIDUAL_H

#include <string>
#include <unordered_set>
#include <vector>
#include <queue>
#include <cmath>
#include <numeric>
#include <utility>
#include <algorithm>
#include <sstream>
#include "Services.h"
#include <vector>

class HouseMaster;
class HouseMasterAffiliate;

class Individual {
public:

    Individual();

    virtual ~Individual() = default;

    explicit Individual(std::string name);

    virtual std::string getId() const = 0;

    void changeUsername(HouseMaster &hm, std::string newUsername) const;

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
    Collaborator(std::vector<std::string> services, const std::string &name, bool pro, std::vector<Availability> &availabylity, float earnings = 0,
                 Classification score = newHere, std::string affiliate = "");

    ~Collaborator() override = default;

    std::vector<std::string> getServices();

    bool isPro() const;

    bool canPreform(const std::string &service);

    bool isAvailable(HouseMasterAffiliate &hm, const std::string &collabId, Date start, Duration duration) const;

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

    const std::vector<Availability> &getAvailability() const;

    bool operator==(const Collaborator &ind2) const;

    bool operator<(const Collaborator &col2) const;

    class AlreadyKnows;

    class ServiceRequiresPro;

private:

    std::vector<std::string> _services;
    Classification _score;
    bool _pro;
    float _earnings;
    std::vector<Availability> _availability;
    std::vector<Classification> _classifications;
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



class Collaborator_pointer_compare{
public:
    bool operator() (const std::pair<Intervention*, Collaborator*>& col1, const std::pair<Intervention*, Collaborator*>& col2){
        int a = 10;
        int b = 10;
        for(const auto & availability : col1.second->getAvailability()){
            if((col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday < 0) ?
            (7 - (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday)) :
               (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday) < a)
                a = (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday < 0) ?
                    (7 - (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday)) :
                    (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday);

        }

        for(const auto & availability : col2.second->getAvailability()){
            if((col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday < 0) ?
               (7 - (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday)) :
               (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday) < a)
                b = (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday < 0) ?
                    (7 - (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday)) :
                    (col1.first->getStartingTime().getDate().tm_wday - availability.getDate().tm_wday);

        }
        return a < b;
    }
};


// Client code
class Client : public Individual {
public:
    static unsigned int _idSeqClt;

    Client(unsigned int nif, const std::string &name, std::string email, bool premium, std::string affiliate = "");

    ~Client() override = default;

    unsigned int getNif();

    std::string getAffiliate() const;

    bool isPremium() const;

    std::string getId() const override;

    void requestIntervention(HouseMasterAffiliate &hm, const Date &date, const std::string &service, bool forcePro,
                             unsigned int nrOfRooms = 0) const;

    static void cancelIntervention(Intervention *intervention);

    static void classifyCollaborator(HouseMaster* hm, const std::string &collabId, Classification classification);

    bool operator==(const Client &ind2) const;

    void setEmail(const std::string &email);

    std::string getEmail() const;

private:
    unsigned int _nif;
    std::string _email;
    bool _premium;
    std::string _affiliate;


};



// Admin code
class Admin : public Individual {
public:
    Admin();
    Admin(const std::string &name, std::string password, std::vector<std::string> affiliates);

    explicit Admin(const std::string &name);

    ~Admin() override = default;
    std::vector<std::string> getAffiliates() const;
    static unsigned int _idSeqAdmins;
    std::string getId() const override;
    std::string getPassword() const;
    void setPassword(const std::string& password);

private:
    std::string _password;
    std::vector<std::string> _affiliates;
};


#endif
