#include "Services.h"
#include "Individuals.h"

class Client: public Individual
{
public:
    Client(unsigned int nif);
    unsigned int getNif();
    int requestIntervention(date appointment, servicesType type, bool forcePro);
private:
    unsigned int nif;
};