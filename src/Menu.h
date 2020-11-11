#ifndef PROJETO_AEDA_1_HOUSEMASTER_MENU_H
#define PROJETO_AEDA_1_HOUSEMASTER_MENU_H
#include <string>
#include <map>
#include <functional>
#include <utility>
#include <iomanip>
#include <iostream>
#include <limits>
#include <vector>
#include <algorithm>

class Menu {
public:
    Menu() = default;
    Menu(std::string prompt, std::map<std::string, std::function<void()>> options);
    void show();
    void select();
    void execute(bool &running);
private:
    std::string _prompt;
    std::map<std::string, std::function<void()> > _options;
    unsigned _choice{};
};

/*
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
};

*/
#endif //PROJETO_AEDA_1_HOUSEMASTER_MENU_H
