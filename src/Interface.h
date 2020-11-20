
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

    explicit Interface(const HouseMaster &houseMaster);

    void selectRole(bool &running);

    HouseMaster getHouseMasterState() const;

private:
    void userLogin();

    void readRole(const std::string &username);

    void adminLogin();

    void clientOperations(bool &running);

    void collaboratorOperations(bool &running);

    static Date readInterventionDate();

    std::string selectService(bool &running);

    static void show(const Service &service);

    static void show(const Collaborator &collaborator);

    void show(Intervention &intervention);

    void showPayment(Intervention *intervention);

    static Classification readClassification(bool &running);

    void adminOperations(bool &running);

    std::string selectCollab(bool &running);

    void readNewCollaboratorData(bool &running);

    std::string readNewServiceData(bool &running);

    Intervention *selectActiveIntervention(bool &running);

    class NonexistentRole;

    static bool isValidNif(unsigned nif);

    class InvalidNif;

    void showFinances() const;

    void readNewClientData();

    static unsigned readNumberOfRooms();

    void showSortedCollabs();

    HouseMaster _houseMaster;
    Individual *_user{};
    Role _role{};
};

class Interface::NonexistentRole : public std::logic_error {
public:
    explicit NonexistentRole(const std::string &error_msg);
};


class Interface::InvalidNif : public std::invalid_argument {
public:
    explicit InvalidNif(const std::string &error_msg);
};

#endif //PROJETO_AEDA_1_HOUSEMASTER_INTERFACE_H
