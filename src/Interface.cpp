#include "Interface.h"

#include <utility>

Interface::NonexistentRole::NonexistentRole(const std::string &error_msg) : std::logic_error(error_msg){}

Interface::Interface(HouseMaster houseMaster) : _houseMaster(std::move(houseMaster)), _user(), _role() {

}

void Interface::selectRole(bool &running) {
    bool innerRunning = true;
    Menu roles("Choose your role", {{"Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
        while (innerRunning) {
            adminOperations(innerRunning);
        }
    }},
    {"User (Collaborator/Client)", [&]() {
        userLogin();
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        if (_role == client) {
            while (innerRunning) {
                clientOperations(innerRunning);
            }
        } else if (_role == collaborator) {
            while (innerRunning) {
                collaboratorOperations(innerRunning);
            }
        }


    }}});

    roles.show();
    roles.select();
    roles.execute(running);

}


void Interface::adminLogin() {
    std::string password{};
    std::cout << "Password : "; std::cin >> password;
    for (int i = 0; i <= 1; i++)
    {
        if (password=="admin")
        {
            std::cout << "success \n";
            _role = admin;
            return;
        }
        else
        {
            std::cout << "Wrong password. Try again:\n";
            std::cin >> password;
        }
    }
    std::cout << "Too many tries! No admin for you. :^) Logging out...\n";
    exit(0);
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
        if (isalpha(i)) roleName += i;  // ai isto ta xanxo que fode e nem funciona bem
        else break;
    }
    if (roleName == "collab") role = collaborator;
    else if (roleName == "client") role = client;
    else {
        throw NonexistentRole("Role does not exist!");
    }
    _role = role;
    return true;
}

void Interface::clientOperations(bool &running) {
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
    }}, {"See active interventions", [&](){
        while (innerRunning) {
            Intervention* intervention = selectActiveIntervention(innerRunning);
            if (intervention) {
                Menu activeInterventionMenu("Active intervention", {{"Mark as done", [&](){
                    // mark as complete, prompt client to pay, prompt client to classify
                    _houseMaster.markAsComplete(intervention);
                    showPayment(intervention);
                    std::cin.ignore();
                    _houseMaster.processTransaction(intervention);
                    Classification classification = readClassification(innerRunning);
                    _houseMaster.getCollaborators()[intervention->getCollabId()]->addClassification(classification);
                }},{"Cancel Intervention", [&](){
                    Client::cancelIntervention(intervention);
                }}, {"See details", [&](){
                    if (!intervention->getService()->getName().empty()) show(*intervention);
                    std::cin.ignore();
                }}});
                activeInterventionMenu.show();
                activeInterventionMenu.select();
                activeInterventionMenu.execute(running);
            }
        }
    }}});
    clientMenu.show();
    clientMenu.select();
    clientMenu.execute(running);
}

void Interface::collaboratorOperations(bool &running) {
    auto * collab = dynamic_cast<Collaborator*>(_user);
    bool innerRunning = true;
    Menu collabsMenu("Welcome, " + _user->getName(), {{"See profile",[&](){
        show(*collab);
        std::cin.ignore();
    }}, {"See active interventions", [&](){
        Intervention* intervention = selectActiveIntervention(innerRunning);
        if (intervention) {
            Menu activeInterventionMenu("Active intervention", {{"See details", [&](){
                if (!intervention->getService()->getName().empty()) show(*intervention);
                std::cin.ignore();
            }}});
            activeInterventionMenu.show();
            activeInterventionMenu.select();
            activeInterventionMenu.execute(running);
        }
    }}, {"Learn a service", [&](){

        Menu pickServices("Learn a service", {{"Choose from the HouseMaster services", [&](){
            while (running) {
                std::string service = selectService(running);
                if (!service.empty()) {
                    if (!collab->canPreform(service)) collab->addService(service);
                    else std::cout << collab->getName() << " already knows " << service << "\n";
                }
            }
        }}, {"Add a new one", [&](){
            std::string serviceName = readNewServiceData(running);
            collab->addService(serviceName);

        }}});

        while (innerRunning) {
            pickServices.show();
            pickServices.select();
            pickServices.execute(innerRunning);
        }

    }}});
    collabsMenu.show();
    collabsMenu.select();
    collabsMenu.execute(running);
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
    std::string pro = service.getPro() ? "yes" : "no";
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << service.getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Base Price" << "] " << std::setw(16) << std::right << service.getBasePrice() << "€ |" << std::endl;
    std::cout << "| [" << "Duration" << "] " << std::setw(19) << std::right << service.getDuration().durationToStr() << " |" << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(15) << std::right << pro << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

void Interface::adminOperations(bool &running) {

    bool innerRunning = true;
    Menu adminMenu("Welcome, ADMIN", {{"Register collaborator", [&](){
        readNewCollaboratorData(innerRunning);
    }}, {"Show all collaborators", [&](){
        while (innerRunning) {
            std::string collabName = selectCollab(innerRunning);
            if (!collabName.empty()) {
                Collaborator *collab = _houseMaster.getCollaborators()[collabName];
                if (collab) show(*collab);
                std::cin.ignore();
            }
        }
    }}, {"See HouseMaster finances", [&](){
        showFinances();
    }}});
    adminMenu.show();
    adminMenu.select();
    adminMenu.execute(running);
}

void Interface::readNewCollaboratorData(bool &running) {

    std::string name{}, pro{};
    std::vector<std::string> services{};

    std::cout << "Name ? "; std::cin.ignore(9999, '\n'); std::getline(std::cin, name, '\n');

    std::cout << "Pro ? [yes/no] "; std::cin >> pro;
    while (pro != "yes" && pro != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> pro;
    }

    bool innerRunning = true;

    Menu pickServices("Pick your services", {{"Choose from the HouseMaster services", [&](){
        while (running) {
            std::string service = selectService(running);
            if (!service.empty()) services.push_back(service);
        }
    }}, {"Add a new one", [&](){
        std::string serviceName = readNewServiceData(running);
        if (!serviceName.empty()) services.push_back(serviceName);

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
    //duration duration{};

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
    duration duration(durationStr);
    _houseMaster.addAvailableService(name, pro, basePrice, duration);

    return name;

}

std::string Interface::selectCollab(bool &running) {
    auto collabs = _houseMaster.getCollaborators();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : collabs) {
        options.insert(std::pair<std::string, std::function<void()>>(_houseMaster.getCollaborators()[i.first]->getName(), [&selection, &i](){
            selection = i.first;
        }));
    }
    Menu collabsMenu("Select a collaborator", options);
    collabsMenu.show();
    collabsMenu.select();
    collabsMenu.execute(running);
    return selection;
}

Intervention *Interface::selectActiveIntervention(bool &running) {
    auto interventions = _houseMaster.getInterventions();
    std::vector<Intervention*> activeInterventions = _houseMaster.getAssociatedActiveInterventions(_user->getId());
    std::map<std::string, std::function<void()>> options{};
    Intervention* selection{};
    for (const auto &i : activeInterventions) {
        options.insert(std::pair<std::string, std::function<void()>>(i->getService()->getName() + " " + i->getStartingTime().dateToStr(), [&selection, &i](){
            selection = i;
        }));
    }
    Menu activeInterventionsMenu("Select an intervention", options);
    activeInterventionsMenu.show();
    activeInterventionsMenu.select();
    activeInterventionsMenu.execute(running);
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


void Interface::show(Intervention &intervention) {
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << intervention.getService()->getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Starting at" << "] " << std::setw(16) << std::right << intervention.getStartingTime().dateToStr() << " |" << std::endl;
    std::cout << "| [" << "Ending at" << "] " << std::setw(16) << std::right << intervention.getEndTime().dateToStr() << " |" << std::endl;
    std::cout << "| [" << "Cost" << "] " << std::setw(19) << std::right << intervention.getCost() << " |" << std::endl;
    std::cout << "| [" << "Collaborator" << "] " << std::setw(15) << std::right << _houseMaster.getCollaborators()[intervention.getCollabId()]->getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

void Interface::showPayment(Intervention *intervention) {
    float cost = intervention->getCost();
    auto *client = dynamic_cast<Client*> (_user);
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << intervention->getService()->getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Client Name" << "] " << std::setw(16) << std::right << client->getName() << " |" << std::endl;
    std::cout << "| [" << "Client NIF" << "] " << std::setw(16) << std::right << client->getNif() << " |" << std::endl;
    std::cout << "| [" << "Total cost" << "] " << std::setw(16) << std::right << cost << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Confirm                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

Classification Interface::readClassification(bool &running) {
    Classification classification{};
    Menu classifications("Review intervention", {{"Unreliable", [&classification](){
        classification = unreliable;
    }}, {"Clumsy", [&classification](){
        classification = clumsy;
    }}, {"Gets it done", [&classification] () {
        classification = getsItDone;
    }}, {"Hardworking", [&classification](){
        classification = hardWorking;
    }}, {"Attentive", [&classification](){
        classification = attentive;
    }}, {"Savior", [&classification](){
        classification = savior;
    }}});
    classifications.show();
    classifications.select();
    classifications.execute(running);
    return classification;
}

HouseMaster Interface::getHouseMasterState() const {
    return _houseMaster;
}

void Interface::showFinances() const {
    float money = _houseMaster.getEarnings();
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << "HouseMaster finances"<< " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Current Balance" << "] " << std::setw(16) << std::right << money << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Confirm                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}






