
#include "Interface.h"

#include <utility>

Interface::Interface(HouseMaster houseMaster) : _houseMaster(std::move(houseMaster)), _user(), _role() {

}

void Interface::selectRole(bool &running) {
    bool innerRunning = true;
    Menu roles("Choose your role", {{"Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
    }}, {"User (Collaborator/Client)", [&]() {
        userLogin();
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        while (innerRunning) {
            clientOpperations(innerRunning);
        }
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

void Interface::clientOpperations(bool &running) {
    auto * client = dynamic_cast<Client*>(_user);
    bool innerRunning = true;
    Menu clientMenu("Welcome" + client->getName(), {{"Request an Intervention", [&](){
        while (innerRunning) {
            Service service = selectService(innerRunning);
            if (!service.name.empty()) {
                date interventionDate = readInterventionDate();
                client->requestIntervention(interventionDate, service);
            }
        }
    }}, {"Browse Services", [&](){
        while (innerRunning) {
            Service service = selectService(innerRunning);
            if (!service.name.empty()) showService(service);
            std::cin.ignore();
        }
    }}, {"Logout", [&](){
        running= false;
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

Service Interface::selectService(bool &running) {
    std::vector<Service* > services = _houseMaster.getAvailableServices();
    Service* selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : services) {
        options.insert(std::pair<std::string, std::function<void()>>(i->name, [&selection, &i](){
            selection = i;
            std::cout << "Selected " << i->name << "\n";
        }));
    }
    std::string goBack;
    std::stringstream ss{}; ss << "Go Back"; goBack = ss.str();
    options.insert(std::pair<std::string, std::function<void()>>(goBack, [&running, &selection](){
        running = false;
        selection = new Service();
    }));
    Menu servicesMenu("Select a service", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute();
    return *selection;
}

void Interface::showService(Service service) {
    std::string pro = service.pro ? "yes" : "no";
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << service.name << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Base Price" << "] " << std::setw(16) << std::right << service.basePrice << "€ |" << std::endl;
    std::cout << "| [" << "Duration" << "] " << std::setw(19) << std::right << service.duration.dateToStr() << " |" << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(15) << std::right << pro << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "| [0] Exit Program               |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

