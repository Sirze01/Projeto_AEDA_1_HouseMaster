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


#endif //PROJETO_AEDA_1_HOUSEMASTER_MENU_H
