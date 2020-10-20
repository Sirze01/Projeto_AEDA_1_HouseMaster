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