
#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H

#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <set>
#include "Individual.h"
#include "Services.h"
#include "Collaborator.h"
#include "Client.h"


class HouseMaster {
public:
    HouseMaster() = default;

    HouseMaster(std::ifstream collaborators, std::ifstream clients);

    const std::vector<Collaborator *> &getCollaborators() const;

    const std::vector<Client *> &getClients() const;

    const std::vector<servicesType *> &getAvailableServices() const;

    const std::set<Intervention *> &getInterventions() const;

    void addAvailableService(servicesType *service);

    void removeAvailableService(servicesType *service);

    void updateInterventions();

private:
    std::vector<Collaborator* > _collaborators;
    std::vector<Client* > _clients;
    std::vector<servicesType* > _availableServices;
    std::set<Intervention* > _interventions;
};


#endif //SRC_HOUSEMASTER_H
