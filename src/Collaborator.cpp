#include "Collaborator.h"

std::vector<servicesType*> Collaborator::getFunctions()
{
    return functions;
}

int Collaborator::getScore()
{
    return score;
}

int Collaborator::getAvailability()
{
    return availability;
}

Collaborator::Collaborator(const std::vector<servicesType *>& functions, const std::string& name) : Individual(name),
    functions(functions), availability(true), score(newHere) {

}
