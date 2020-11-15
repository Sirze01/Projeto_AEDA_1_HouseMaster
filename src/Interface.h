
#ifndef PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H
#define PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H

#include "HouseMaster.h"
#include "Menu.h"

enum Role {
    admin,
    collaborator,
    client
};

class Interface {

public:
    Interface() = default;
    explicit Interface(HouseMaster houseMaster);
    void selectRole(bool &running);
    void userLogin();
    bool readRole(const std::string &username);
    void adminLogin();
    void clientOpperations(bool &running);
    date readInterventionDate();
    std::string selectService(bool &running);
    void show(const Service& service);
    void show(const Collaborator& collaborator);
    void adminOpperations(bool &running);
    std::string selectCollab(bool &running);
    void readNewCollaboratorData(bool &running);
    std::string readNewServiceData(bool &running);
    Intervention* selectActiveIntervention(bool &running);
private:
    HouseMaster _houseMaster;
    Individual *_user;
    Role _role;
};


#endif //PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H
