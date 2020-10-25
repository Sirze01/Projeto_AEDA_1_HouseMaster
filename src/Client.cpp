#include "Client.h"

Client::Client(unsigned int nif, const std::string &name) : Individual(name), _nif(nif), _requestedInterventions() {}

unsigned int Client::getNif()
{
    return(_nif);
}

int Client::requestIntervention(date appointment, servicesType type, bool forcePro, float price)
{
    auto intervention = new Intervention(appointment, type, forcePro, price);
    _requestedInterventions.push_back(intervention);
    return 0;
}

const std::vector<Intervention *> &Client::getRequestedInterventions() const {
    return _requestedInterventions;
}
