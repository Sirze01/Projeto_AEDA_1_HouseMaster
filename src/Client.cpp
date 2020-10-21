#include "Client.h"

Client::Client(unsigned int nif, const std::string &name) : Individual(name), _nif(nif) {}

unsigned int Client::getNif()
{
    return(_nif);
}

int Client::requestIntervention(date appointment, servicesType type, bool forcePro)
{
    Intervention(appointment, type, forcePro);
    return 0;
}