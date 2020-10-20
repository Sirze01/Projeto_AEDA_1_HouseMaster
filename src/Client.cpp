#include "Client.h"

Client::Client(unsigned int nif) : Individual(_name)
{
    this->nif = nif;
}

unsigned int Client::getNif()
{
    return(nif);
}

int Client::requestIntervention(date appointment, servicesType type, bool forcePro)
{
    Intervention(appointment, type, forcePro);
    return 0;
}