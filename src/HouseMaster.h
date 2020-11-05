#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include "Individual.h"
#include "Services.h"
#include <utility>
#include <stdexcept>
#include <iostream>


class HouseMaster {
public:
    HouseMaster();

    HouseMaster(std::ifstream collaborators, std::ifstream clients, std::ifstream services);

    const std::vector<Collaborator *> &getCollaborators() const;

    const std::vector<Client *> &getClients() const;

    const std::vector<Service *> &getAvailableServices() const;

    const std::set<std::pair<Client *, Intervention *> > &getInterventions() const;

    void addAvailableService(Service *service);

    void removeAvailableService(Service *service);

    void updateInterventions();

    void assignColaborator(Intervention* intervention);

    void sortCollaboratorsByScore();

    Service* findServiceByName(const std::string &name);

    Client* findClientByUniqueName(const std::string &name);

    Collaborator* findCollabByUniqueName(const std::string &name);

    class UnavailableAppointment;

    class InexistentService;

private:
    std::vector<Collaborator *> _collaborators;
    std::vector<Client *> _clients;
    std::vector<Service *> _availableServices;
    std::set<std::pair<Client *, Intervention *> > _interventions;
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
