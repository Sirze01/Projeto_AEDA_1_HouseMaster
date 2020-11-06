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

bool colComparer(std::pair<std::string, Collaborator*>& a, std::pair<std::string, Collaborator*>& b){
    return a.second->getScore() > b.second->getScore();
}


class HouseMaster {
public:
    HouseMaster();

    HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services);

    std::map<std::string, Collaborator *>& getCollaborators() ;

    std::unordered_map<std::string, Client *>& getClients() ;

    std::vector<Intervention *>& getInterventions() ;

    void addAvailableService(const std::string& name, bool pro, float basePrice, date duration); //DONE

    void removeAvailableService(const std::string& serviceName);                                 //DONE

    std::unordered_map<std::string, Service*>& getAvailableServices();

    void addCollaborator(const std::vector<std::string>& functions, const std::string &name, bool pro);

    void addCollaborator(const std::string& username, Collaborator* collaborator);

    void removeCollaborator(const std::string& collId);

    void deleteCollaborator(const std::string& collId);

    void addClient(unsigned int nif, const std::string &name, bool premium);

    void removeClient(const std::string& clientId);

    void addIntervention(const date& appointment, const std::string& type, bool forcePro);

    void assignColaborator(Intervention*, const std::vector<std::pair<std::string, Collaborator*>>&);

    std::vector<std::pair<std::string, Collaborator*>> sortCollaboratorsByScore();

    Individual* findByUsername(const std::string &name);

    class UnavailableAppointment;

    class InexistentService;

private:
    std::unordered_map<std::string, std::string> _usernameMap;
    std::map<std::string, Collaborator*> _collaborators;
    std::unordered_map<std::string, Client*> _clients;
    std::unordered_map<std::string, Service*> _availableServices;
    std::vector<Intervention*> _interventions;
};


class HouseMaster::InexistentService : public std::out_of_range {
public:
    explicit InexistentService(const std::string &error_msg);
};

class HouseMaster::UnavailableAppointment: public std::logic_error{
public:
    explicit UnavailableAppointment(const std::string &error_msg);
};




#endif //SRC_HOUSEMASTER_H
