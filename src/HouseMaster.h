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


/**
 *
 * */
bool colComparer(std::pair<std::string, Collaborator*>& a, std::pair<std::string, Collaborator*>& b);

/**\class HouseMaster
 * \brief a class that menages the House Master
 * This class manages the services, collaborators, clients and finances*/
class HouseMaster {
public:
    /**HouseMaster's empty constructor*/
    HouseMaster();
    /**HouseMaster's constructor using files*/
    HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services);
    /**A normal member taking no arguments and returning an unordered map of Collaborators
     * \return Unordered map of collaborators*/
    std::unordered_map<std::string, Collaborator *>& getCollaborators();
    /**A normal member taking no arguments and returning an unordered map of Clients
     * \return Unordered map of clients*/
    std::unordered_map<std::string, Client *>& getClients();
    /**A normal member taking no arguments and returning a vector of interventions
     * \return Unordered map of collaborators*/
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
    //Intervention* addIntervention(const date& appointment, const std::string& type, bool forcePro, const std::string &clientId);
    Intervention* addIntervention(const date& appointment, const std::string& type, bool forcePro, const std::string &clientId, unsigned int nrOfRooms=0);
    static void changeinterventionState(Intervention* intervention, processState state);
    void processTransaction(Intervention *intervention);
    std::vector<Intervention*> getAssociatedInterventions(const std::string& id);
    std::vector<Intervention*> getAssociatedActiveInterventions(const std::string& id);
    void assignColaborator(Intervention*, const std::vector<std::pair<std::string, Collaborator*>>&);
    std::vector<std::pair<std::string, Collaborator*>> sortCollaboratorsByScore();
    Individual* findByUsername(const std::string &name);
    void markAsComplete(Intervention* intervention);
    float getEarnings() const;
    class UnavailableAppointment;
    class InexistentService;
    class ExistentService;
    class InexistentCollab;
    class InexistentClient;
    class ExistentClient;
    class NonexistentUsername;
    class UnableToWriteFile;
    void addAvailableService(Service *service);
    void writeCollabsInfo();
    void writeClientsInfo();
    void writeServicesInfo();
    void writeInterventionsInfo();
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


class HouseMaster::NonexistentUsername: public std::out_of_range
{
public:
    explicit NonexistentUsername(const std::string &error_msg);
};

class HouseMaster::UnableToWriteFile: public std::ios_base::failure{
public:
    explicit UnableToWriteFile(const std::string &error_msg);
};

#endif //SRC_HOUSEMASTER_H
