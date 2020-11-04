
#include "Interface.h"

#include <utility>

Interface::Interface(HouseMaster houseMaster) : _houseMaster(std::move(houseMaster)), _user(), _role() {

}

void Interface::selectRole() {

    Menu roles("Choose your role", {{"Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        _role = admin;
        adminLogin();
    }}, {"Collaborator", [&]() {
        std::cout << "Welcome to HouseMaster. You are a COLLABORATOR.\n";
        _role = collaborator;
        collabLogin();
    }}, {"Client", [&](){
        std::cout << "Welcome to HouseMaster. You are a CLIENT.\n";
        _role = client;
        clientLogin();
    }}});

    roles.show();
    roles.select();
    roles.execute();

}

void Interface::clientLogin() {
    std::string username{};
    std::cout << "Username : "; std::cin >> username;
    _user = _houseMaster.findClientByUniqueName(username);
    std::cout << "success for " << _user->getName() << "\n";
}

void Interface::collabLogin() {
    std::string username{};
    std::cout << "Username : "; std::cin >> username;
    _user = _houseMaster.findCollabByUniqueName(username);
    std::cout << "success for " << _user->getName() << "\n";
}

void Interface::adminLogin() {
    std::string password{};
    std::cout << "Password : "; std::cin >> password;
    if (password=="admin") std::cout << "success \n";
    else std::cout << "Wrong password. No admin for you. :^)";
}
