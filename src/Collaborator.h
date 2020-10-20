#include "Individual.h"
#include "Services.h"
#include <vector>

enum Classification {
    unreliable,
    clumsy,
    newHere,
    getsItDone,
    hardWorker,
    attentive,
    savior
};


class Collaborator: public Individual
{
public:
    Collaborator(const std::vector<servicesType*>& functions, const std::string& name);
    std::vector<servicesType*> getFunctions();
    int getScore();
    int getAvailability();
private:
    std::vector<servicesType*> functions;
    Classification score;
    bool availability;
};