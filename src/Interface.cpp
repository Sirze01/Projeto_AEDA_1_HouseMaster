
#include "Interface.h"

#include <utility>

Interface::Interface(HouseMaster houseMaster) : _houseMaster(std::move(houseMaster)), _user(), _role() {

}

void Interface::selectRole(bool &running) {
    bool innerRunning = true;
    Menu roles("Choose your role", {{"Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
        while (innerRunning) {
            adminOpperations(innerRunning);
        }
    }},
    {"User (Collaborator/Client)", [&]() {
        userLogin();
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        while (innerRunning) {

            clientOpperations(innerRunning);
        }
    }}});

    roles.show();
    roles.select();
    roles.execute(running);

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
    _user = _houseMaster.findByUsername(username);
}

bool Interface::readRole(const std::string &username) {
    std::string roleName{};
    Role role{};
    for (const auto &i : username) {  // muito xanxo melhorar
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
    Menu clientMenu("Welcome, " + client->getName(), {{"Request an Intervention", [&]() {
        while (innerRunning) {
            std::string service = selectService(innerRunning);
            if (!service.empty()) {
                date interventionDate = readInterventionDate();
                client->requestIntervention(_houseMaster, interventionDate, service, false);
            }
        }
    }}, {"Browse Services", [&]() {
        while (innerRunning) {
            std::string serviceName = selectService(innerRunning);
            Service *service = _houseMaster.getAvailableServices()[serviceName];
            if (!serviceName.empty()) show(*service);
            std::cin.ignore();
        }
    }}});
    clientMenu.show();
    clientMenu.select();
    clientMenu.execute(running);
}

date Interface::readInterventionDate() {
    std::string dateString{};
    std::cin.ignore();
    std::cout << "Insert the desired intervention date in format DD/MM/YYYY HH:mm\n"; std::getline(std::cin, dateString);
    date interventionDate(dateString);
    return interventionDate;
}

std::string Interface::selectService(bool &running) {
    auto services = _houseMaster.getAvailableServices();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : services) {
        options.insert(std::pair<std::string, std::function<void()>>(i.first, [&selection, &i](){
            std::cout << "Selected " << i.first << "\n";
            selection = i.first;
        }));
    }
    Menu servicesMenu("Select a service", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}

void Interface::show(const Service& service) {
    std::string pro = service.pro ? "yes" : "no";
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << service.name << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Base Price" << "] " << std::setw(16) << std::right << service.basePrice << "€ |" << std::endl;
    std::cout << "| [" << "Duration" << "] " << std::setw(19) << std::right << service.duration.dateToStr() << " |" << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(15) << std::right << pro << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

void Interface::adminOpperations(bool &running) {

    bool innerRunning = true;
    Menu adminMenu("Welcome, ADMIN", {{"Register collaborator", [&](){
        readNewCollaboratorData(innerRunning);

    }}, {"Show all collaborators", [&](){
        while (innerRunning) {
            std::string collabName = selectCollab(innerRunning);
            Collaborator *collab = _houseMaster.getCollaborators()[collabName];
            if (!collabName.empty()) show(*collab);
            std::cin.ignore();
        }
    }}});
    adminMenu.show();
    adminMenu.select();
    adminMenu.execute(running);
}

void Interface::readNewCollaboratorData(bool &running) {

    std::string name{}, pro{};
    std::vector<std::string> services{};

    std::cout << "Name ? "; std::cin.ignore(9999, '\n'); std::getline(std::cin, name, '\n');
    std::cout << "PEAD NAME " << name << "\n\n";

    std::cout << "Pro ? [yes/no] "; std::cin >> pro;
    while (pro != "yes" && pro != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> pro;
    }

    bool innerRunning = true;

    Menu pickServices("Pick your services", {{"Choose from the HouseMaster services", [&](){
        while (running) {
            std::string service = selectService(running);
            if (service.empty()) services.push_back(service);
        }
    }}, {"Add a new one", [&](){
        std::string serviceName = readNewServiceData(running);
        services.push_back(serviceName);

    }}});

    while (innerRunning) {
        pickServices.show();
        pickServices.select();
        pickServices.execute(innerRunning);
    }

    _houseMaster.addCollaborator(services, name, pro=="true");

}

std::string Interface::readNewServiceData(bool &running) {
    std::string name{}, proStr{}, durationStr;
    float basePrice{};
    date duration{};

    std::cout << "Name ? "; std::cin.ignore(); std::getline(std::cin, name, '\n');

    std::cout << "Pro ? [yes/no] "; std::cin >> proStr;
    while (proStr != "yes" && proStr != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> proStr;
    }

    bool pro = proStr == "yes";

    std::cout << "Base price ? "; std::cin >> basePrice;
    while (true) {
        if (std::cin.fail() || std::cin.peek() != '\n') {
            if (std::cin.eof()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "User chose to close the input.\n";
                break;
            } else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Invalid choice. Make sure you are entering a valid number: " << std::endl;
                std::cin >> basePrice;
            }
        } else if (basePrice < 0) {
            std::cout << "Invalid choice. Make sure the price is a positive float: " << std::endl;
            std::cin >> basePrice;
        } else {
            break;
        }
    }

    std::cout << "Mean duration : "; std::cin >> durationStr;
    duration.readDuration(durationStr);
    _houseMaster.addAvailableService(name, pro, basePrice, duration);

    return name;

}

std::string Interface::selectCollab(bool &running) {
    auto collabs = _houseMaster.getCollaborators();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : collabs) {
        options.insert(std::pair<std::string, std::function<void()>>(_houseMaster.getCollaborators()[i.first]->getName(), [&selection, &i](){
            std::cout << "Selected " << i.first << "\n";
            selection = i.first;
        }));
    }
    Menu servicesMenu("Select a collaborator", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}

void Interface::show(const Collaborator &collaborator) {
    std::string pro = collaborator.isPro() ? "yes" : "no";
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << collaborator.getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "ID" << "] " << std::setw(16) << std::right << collaborator.getId() << " |" << std::endl;
    std::cout << "| [" << "Score" << "] " << std::setw(19) << std::right << collaborator.getScore() << " |" << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(15) << std::right << pro << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}



