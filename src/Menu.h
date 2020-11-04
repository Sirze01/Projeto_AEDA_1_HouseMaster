#ifndef PROJETO_AEDA_1_HOUSEMASTER_MENU_H
#define PROJETO_AEDA_1_HOUSEMASTER_MENU_H
#include <string>

class Menu
{
public:
    int initialMenu();
    int checkAdmin();
    int checkCollab();
    int checkClient();
    int adminMenu();
    int collabMenu();
    int clientMenu();
    void exitMenu();
    int newCollab();
    int newClient();

private:
    int individualType = 0;         // 1 - admin; 2 - collab; 3 - client
}


#endif //PROJETO_AEDA_1_HOUSEMASTER_MENU_H
