#include "Individuals.h"
#include "Services.h"
#include <vector>

class Collaborator: public Individual
{
public:
    Collaborator(std::vector<servicesType*> functions) : Individual(_name) ;
    std::vector<servicesType*> getFunctions();
    int getScore();
    int getAvailability();
private:
    std::vector<servicesType*> functions;
    classification score;
    bool availability;
};