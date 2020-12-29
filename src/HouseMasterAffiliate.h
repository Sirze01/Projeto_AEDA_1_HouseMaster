#ifndef PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERAFFILIATE_H
#define PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERAFFILIATE_H


#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <map>
#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <iostream>
#include "Individual.h"
#include "Services.h"


bool scoreComparer(std::pair<std::string, Collaborator *> &a, std::pair<std::string, Collaborator *> &b);

/**
 * @brief This class manages the services, collaborators, clients and finances of HouseMaster
 */
class HouseMasterAffiliate {
public:

    HouseMasterAffiliate();

    HouseMasterAffiliate(std::ifstream usernames, std::ifstream collaborators, std::ifstream clients, std::ifstream services,
                                               std::ifstream earnings, std::ifstream history, std::string location, const std::string& responsible,
                         const std::string& hmName);

    ~HouseMasterAffiliate() = default;

    std::map<std::string, Collaborator *> &getCollaborators();

    std::map<std::string, Client *> getClients() const;

    std::unordered_set<Intervention *> & getInterventions();

    void addAvailableService(const std::string &name, bool pro, float basePrice, const Duration &duration);

    void removeAvailableService(const std::string &service);

    std::unordered_map<std::string, Service *> &getAvailableServices();

    void usernameMapChanger(std::string id, std::string newUsername);

    void removeCollaborator(const std::string &collId);

    void addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro, float earnings = 0,
                         Classification score = newHere, std::string affiliate = "");

    void addClient(unsigned long nif, const std::string &name, bool premium, std::string affiliate);

    void removeClient(const std::string &clientId);

    Intervention *
    addIntervention(const Date &start, const std::string &service, bool forcePro, const std::string &clientId,
                    unsigned int nrOfRooms = 0);

    static void changeInterventionState(Intervention *intervention, processState state);

    void processTransaction(Intervention *intervention);

    std::unordered_set<Intervention *> getAssociatedInterventions(const std::string &id);

    std::unordered_set<Intervention *> getAssociatedActiveInterventions(const std::string &id);

    std::unordered_set<Intervention *> getAssociatedPastInterventions(const std::string &id);

    std::unordered_set<Intervention *> getAllPastInterventions();

    std::unordered_set<Intervention *> getAllActiveInterventions();

    std::string getAffiliateName() const;

    Admin getAdmin() const;

    std::string getLocation() const;

    std::string getResponsible() const;

    void assignCollaborator(Intervention *intervention,
                            const std::vector<std::pair<std::string, Collaborator *>> &orderedCollabs);

    std::vector<std::pair<std::string, Collaborator *>> sortCollaboratorsByScore();

    Individual *findByUsername(const std::string &name);

    static void markAsComplete(Intervention *intervention);

    float getEarnings() const;

    class UnavailableAppointment;

    class NonexistentService;

    class ExistentService;

    class UsernameAlreadyInUse;

    class NonexistentCollab;

    class AssignedCollab;

    class NonexistentClient;

    class ExistentClient;

    class NonexistentUsername;

    class UnableToWriteFile;

    void writeUsernameMap();

    void writeCollabsInfo();

    void writeClientsInfo();

    void writeServicesInfo();

    void writeInterventionsInfo();

    void writeFinancialInfo() const;

    void addAvailablePaintService(const std::string &name, bool pro, float basePrice, const Duration &duration);

    bool operator<(const HouseMasterAffiliate &hma) const;

    static unsigned int _idSeqAffiliate;

private:
    std::unordered_map<std::string, Service *> _availableServices;
    std::map<std::string, Client *> _clients;
    std::unordered_map<std::string, std::string> _usernameMap;
    std::map<std::string, Collaborator *> _collaborators;
    std::unordered_set<Intervention *> _interventions;
    float _earnings;
    std::string _name;
    std::string _location;
    unsigned int _id{};
    Admin _responsible;  // TODO discuss if need admin class
    // std::string _responsible;  // TODO: make this a Admin
};

// Must add in the cpp also the definition of the Client::requestIntervention method to complete the forward declaration

class HouseMasterAffiliate::UnavailableAppointment : public std::logic_error {
public:
    explicit UnavailableAppointment(const std::string &error_msg);
};

class HouseMasterAffiliate::NonexistentService : public std::out_of_range {
public:
    explicit NonexistentService(const std::string &error_msg);
};

class HouseMasterAffiliate::ExistentService : public std::out_of_range {
public:
    explicit ExistentService(const std::string &error_msg);
};

class HouseMasterAffiliate::UsernameAlreadyInUse : public std::logic_error{
public:
    explicit UsernameAlreadyInUse(const std::string &error_msg);
};

class HouseMasterAffiliate::NonexistentCollab : public std::out_of_range {
public:
    explicit NonexistentCollab(const std::string &error_msg);
};

class HouseMasterAffiliate::AssignedCollab : public std::logic_error {
public:
    explicit AssignedCollab(const std::string &error_msg);
};

class HouseMasterAffiliate::NonexistentClient : public std::out_of_range {
public:
    explicit NonexistentClient(const std::string &error_msg);
};

class HouseMasterAffiliate::ExistentClient : public std::out_of_range {
public:
    explicit ExistentClient(const std::string &error_msg);
};


class HouseMasterAffiliate::NonexistentUsername : public std::out_of_range {
public:
    explicit NonexistentUsername(const std::string &error_msg);
};

class HouseMasterAffiliate::UnableToWriteFile : public std::ios_base::failure {
public:
    explicit UnableToWriteFile(const std::string &error_msg);
};

#endif //PROJETO_AEDA_1_HOUSEMASTER_HOUSEMASTERAFFILIATE_H
