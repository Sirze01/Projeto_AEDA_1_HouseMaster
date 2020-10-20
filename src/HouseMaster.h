
#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H

#include <vector>
#include <fstream>
#include <sstream>
#include "Individual.h"
#include "Services.h"
#include "Collaborator.h"


class HouseMaster {
public:
    HouseMaster() = default;

    HouseMaster(std::ifstream collaborators, std::ifstream clients);

    const std::vector<Collaborator *> &getCollaborators() const;

    const std::vector<Individual *> &getClients() const;

    const std::vector<servicesType *> &getAvailableServices() const;

    const std::vector<Intervention *> &getInterventions() const;

    void addAvailableService(std::string service, bool pro=false);

    void removeAvailableService(std::string service);

private:
    std::vector<Collaborator* > _collaborators;
    std::vector<Individual* > _clients;
    std::vector<servicesType* > _availableServices;
    std::vector<Intervention* > _interventions;
};


#endif //SRC_HOUSEMASTER_H
