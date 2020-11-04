
#include "Interface.h"

#include <utility>

Interface::Interface(HouseMaster houseMaster) : _houseMaster(std::move(houseMaster)), _user(), _role() {

}

void Interface::selectRole() {

    Menu roles("Choose your role", {{"Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
    }}, {"User (Collaborator/Client)", [&]() {
        userLogin();
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        clientOpperations();
    }}});

    roles.show();
    roles.select();
    roles.execute();

}


void Interface::adminLogin() {
    std::string password{};
    std::cout << "Password : "; std::cin >> password;
    if (password=="admin") std::cout << "success \n";
    else std::cout << "Wrong password. No admin for you. :^)";
    _role = admin;
}

void Interface::userLogin() {
    std::string username{};
    std::cout << "Username : "; std::cin >> username;
    while (!readRole(username)) {
        std::cout << "Login Failed, try again\n";
        std::cout << "Username : "; std::cin >> username;
    }
    if (_role == collaborator) {
        _user = _houseMaster.findCollabByUniqueName(username);
    }
    else if (_role == client) _user = _houseMaster.findClientByUniqueName(username);

}

bool Interface::readRole(const std::string &username) {
    std::string roleName{};
    Role role{};
    for (const auto &i : username) {
        if (isalpha(i)) roleName += i;
        else break;
    }
    if (roleName == "collab") role = collaborator;
    else if (roleName == "client") role = client;
    else {
        // later make an exception!
        return false;
    }
    _role = role;
    return true;
}

void Interface::clientOpperations() {
    auto * client = dynamic_cast<Client*>(_user);
    Menu clientMenu("Welcome" + client->getName(), {{"Request an Intervention", [&](){
        Service service = selectService();
        date interventionDate = readInterventionDate();
        client->requestIntervention(interventionDate, service);
    }}});
    clientMenu.show();
    clientMenu.select();
    clientMenu.execute();
}

date Interface::readInterventionDate() {
    std::string dateString{};
    std::cin.ignore();
    std::cout << "Insert the desired intervention date in format DD/MM/YYYY HH:mm\n"; std::getline(std::cin, dateString);
    date interventionDate(dateString);
    return interventionDate;
}

Service Interface::selectService() {
    std::vector<Service* > services = _houseMaster.getAvailableServices();
    Service* selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : services) {
        options.insert(std::pair<std::string, std::function<void()>>(i->name, [&services, &selection, &i](){
            selection = i;
            std::cout << "Selected " << i->name << "\n";
        }));
    }
    Menu servicesMenu("Select a service", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute();
    return *selection;
}

