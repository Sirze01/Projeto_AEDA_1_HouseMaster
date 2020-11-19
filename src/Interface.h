
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
    void clientOperations(bool &running);
    void collaboratorOperations(bool &running);
    date readInterventionDate();
    std::string selectService(bool &running);
    void show(const Service& service);
    void show(const Collaborator& collaborator);
    void show(Intervention& intervention);
    void showPayment(Intervention* intervention);
    Classification readClassification(bool &running);
    void adminOperations(bool &running);
    std::string selectCollab(bool &running);
    void readNewCollaboratorData(bool &running);
    std::string readNewServiceData(bool &running);
    Intervention* selectActiveIntervention(bool &running);
    class NonexistentRole;
    HouseMaster getHouseMasterState() const;
    void showFinances() const;
    void readNewClientData();
    unsigned readNumberOfRooms();
private:
    HouseMaster _houseMaster;
    Individual *_user;
    Role _role;
};

class Interface::NonexistentRole: public std::logic_error{
public:
    explicit NonexistentRole (const std::string &error_msg);
};


#endif //PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H
