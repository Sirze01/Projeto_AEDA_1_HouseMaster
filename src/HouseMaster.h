#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H

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
class HouseMaster {
public:

    HouseMaster();

    HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services, std::ifstream earnings);

    ~HouseMaster() = default;

    std::map<std::string, Collaborator *> &getCollaborators();

    std::map<std::string, Client *> &getClients();

    std::vector<Intervention *> &getInterventions();

    void addAvailableService(const std::string &name, bool pro, float basePrice, const Duration &duration);

    void removeAvailableService(const std::string &service);

    std::unordered_map<std::string, Service *> &getAvailableServices();

    void removeCollaborator(const std::string &collId);

    void addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro, float earnings = 0,
                    Classification score = newHere);

    void addClient(unsigned long nif, const std::string &name, bool premium);

    void removeClient(const std::string &clientId);

    Intervention *
    addIntervention(const Date &start, const std::string &service, bool forcePro, const std::string &clientId,
                    int nrOfRooms = 0);

    static void changeInterventionState(Intervention *intervention, processState state);

    void processTransaction(Intervention *intervention);

    std::vector<Intervention *> getAssociatedInterventions(const std::string &id);

    std::vector<Intervention *> getAssociatedActiveInterventions(const std::string &id);

    void assignCollaborator(Intervention *intervention,
                            const std::vector<std::pair<std::string, Collaborator *>> &orderedCollabs);

    std::vector<std::pair<std::string, Collaborator *>> sortCollaboratorsByScore();

    Individual *findByUsername(const std::string &name);

    static void markAsComplete(Intervention *intervention);

    float getEarnings() const;

    class UnavailableAppointment;

    class NonexistentService;

    class ExistentService;

    class NonexistentCollab;

    class AssignedCollab;

    class NonexistentClient;

    class ExistentClient;

    class NonexistentUsername;

    class UnableToWriteFile;

    void writeCollabsInfo();

    void writeClientsInfo();

    void writeServicesInfo();

    void writeInterventionsInfo();

    void writeFinancialInfo() const;

    void addAvailablePaintService(const std::string &name, bool pro, float basePrice, const Duration &duration);

private:
    std::unordered_map<std::string, Service *> _availableServices;
    std::map<std::string, Client *> _clients;
    std::unordered_map<std::string, std::string> _usernameMap;
    std::map<std::string, Collaborator *> _collaborators;
    std::vector<Intervention *> _interventions;
    float _earnings;

};

// Must add in the cpp also the definition of the Client::requestIntervention method to complete the forward declaration

class HouseMaster::UnavailableAppointment : public std::logic_error {
public:
    explicit UnavailableAppointment(const std::string &error_msg);
};

class HouseMaster::NonexistentService : public std::out_of_range {
public:
    explicit NonexistentService(const std::string &error_msg);
};

class HouseMaster::ExistentService : public std::out_of_range {
public:
    explicit ExistentService(const std::string &error_msg);
};

class HouseMaster::NonexistentCollab : public std::out_of_range {
public:
    explicit NonexistentCollab(const std::string &error_msg);
};

class HouseMaster::AssignedCollab : public std::logic_error {
public:
    explicit AssignedCollab(const std::string &error_msg);
};

class HouseMaster::NonexistentClient : public std::out_of_range {
public:
    explicit NonexistentClient(const std::string &error_msg);
};

class HouseMaster::ExistentClient : public std::out_of_range {
public:
    explicit ExistentClient(const std::string &error_msg);
};


class HouseMaster::NonexistentUsername : public std::out_of_range {
public:
    explicit NonexistentUsername(const std::string &error_msg);
};

class HouseMaster::UnableToWriteFile : public std::ios_base::failure {
public:
    explicit UnableToWriteFile(const std::string &error_msg);
};

#endif //SRC_HOUSEMASTER_H
