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



bool colComparer(std::pair<std::string, Collaborator*>& a, std::pair<std::string, Collaborator*>& b);

class HouseMaster {
public:
    HouseMaster();

    HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services);

    std::unordered_map<std::string, Collaborator *>& getCollaborators();

    std::unordered_map<std::string, Client *>& getClients();

    std::vector<Intervention *>& getInterventions();

    void addAvailableService(const std::string& name, bool pro, float basePrice, duration duration);

    void removeAvailableService(const std::string& serviceName);

    std::unordered_map<std::string, Service*>& getAvailableServices();

    void addCollaborator(Collaborator *collab);

    //void addCollaborator(const std::string& username, Collaborator* collaborator);

    void removeCollaborator(const std::string& collId);

    void addCollaborator(const std::vector<std::string> &functions, const std::string &name, bool pro);

    //void deleteCollaborator(const std::string& collId);

    void addClient(Client *client);

    void addClient(unsigned int nif, const std::string &name, bool premium);

    void removeClient(const std::string& clientId);

    void addIntervention(const date& appointment, const std::string& type, bool forcePro, const std::string &clientId);

    static void changeinterventionState(Intervention* intervention, processState state);

    static void changeinterventionStatePayed(Intervention* intervention);

    void processTransaction(Intervention *intervention);

    std::vector<Intervention*> getAssociatedInterventions(const std::string& id);

    std::vector<Intervention*> getAssociatedActiveInterventions(const std::string& id);

    void assignColaborator(Intervention*, const std::vector<std::pair<std::string, Collaborator*>>&);

    std::vector<std::pair<std::string, Collaborator*>> sortCollaboratorsByScore();

    Individual* findByUsername(const std::string &name);

    class UnavailableAppointment;

    class InexistentService;

    class ExistentService;

    class InexistentCollab;

    class InexistentClient;

    class ExistentClient;

    void addAvailableService(Service *service);

    void writeCollabsInfo();

    void writeClientsInfo();

    void writeServicesInfo();

private:
    std::unordered_map<std::string, Service*> _availableServices;
    std::unordered_map<std::string, Client*> _clients;
    std::unordered_map<std::string, std::string> _usernameMap;
    std::unordered_map<std::string, Collaborator*> _collaborators;
    std::vector<Intervention*> _interventions;
    float _earnings;

};

// Must add in the cpp also the definition of the Client::requestIntervention method to complete the forward declaration

class HouseMaster::UnavailableAppointment: public std::logic_error{
public:
    explicit UnavailableAppointment(const std::string &error_msg);
};

class HouseMaster::InexistentService : public std::out_of_range {
public:
    explicit InexistentService(const std::string &error_msg);
};

class HouseMaster::ExistentService : public std::out_of_range {
public:
    explicit ExistentService(const std::string &error_msg);
};

class HouseMaster::InexistentCollab : public std::out_of_range{
public:
    explicit InexistentCollab(const std::string &error_msg);
};

class HouseMaster::InexistentClient : public std::out_of_range{
public:
    explicit InexistentClient(const std::string &error_msg);
};

class HouseMaster::ExistentClient : public std::out_of_range{
public:
    explicit ExistentClient(const std::string &error_msg);
};

#endif //SRC_HOUSEMASTER_H
