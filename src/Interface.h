
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
    void selectRole();
    void clientLogin();
    void collabLogin();
    void adminLogin();
private:
    HouseMaster _houseMaster;
    Individual *_user;
    Role _role;

};


#endif //PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H
