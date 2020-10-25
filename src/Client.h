#include "Services.h"
#include "Individual.h"
#include <vector>

class Client: public Individual
{
public:
    Client(unsigned int nif, const std::string &name);
    unsigned int getNif();
    int requestIntervention(date appointment, servicesType type, bool forcePro, float price);

    const std::vector<Intervention *> &getRequestedInterventions() const;

private:
    unsigned int _nif;
    std::vector<Intervention* > _requestedInterventions;
};