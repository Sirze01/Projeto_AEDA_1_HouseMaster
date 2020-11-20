#include "Interface.h"


Interface::NonexistentRole::NonexistentRole(const std::string &error_msg) : std::logic_error(error_msg) {}

Interface::InvalidNif::InvalidNif(const std::string &error_msg) : invalid_argument(error_msg) {}

/**
 * @brief Interface constructor
 * @param houseMaster the housemaster to interact with
 */
Interface::Interface(const HouseMaster &houseMaster) : _houseMaster(houseMaster), _user(), _role() {

}

/**
 * @brief selects a role to interact with
 * @param running
 */
void Interface::selectRole(bool &running) {
    bool innerRunning = true;
    Menu roles("Choose your role", {{"Login Admin", [&]() {
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
        while (innerRunning) {
            adminOperations(innerRunning);
        }
    }}, {"Login User (Collab/Client)", [&]() {
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


    }},
    {"Register Client",            [&]() {
        readNewClientData();
        std::cout << "Press ENTER to continue\n";
        std::cin.ignore();
    }}});

    roles.show();
    roles.select();
    roles.execute(running);

}

/**
 * @brief let's the admin login
 */
void Interface::adminLogin() {
    std::string password{};
    std::cout << "Password : ";
    std::cin >> password;
    for (int i = 0; i <= 1; i++) {
        if (password == "admin") {
            std::cout << "success \n";
            _role = admin;
            return;
        } else {
            std::cout << "Wrong password. Try again:\n";
            std::cin >> password;
        }
    }
    std::cout << "Too many tries! No admin for you. Logging out...\n";
    exit(0);
}

/**
 * @brief let's a user login
 */
void Interface::userLogin() {
    std::string username{};
    std::cout << "Username : ";
    std::cin >> username;
    bool done;
    do {
        try {
            done = true;
            readRole(username);
        }
        catch (const NonexistentRole &e) {
            done = false;
            std::cout << e.what() << " Please try again\n";
            std::cout << "Username : ";
            std::cin >> username;
        }
    } while (!done);
    _user = _houseMaster.findByUsername(username);
}

/**
 * @brief reads the role from username
 * @param username the username
 */
void Interface::readRole(const std::string &username) {
    std::string roleName{};
    Role role;
    if (username == "client") {
        std::cout << "You need to be more specific! Example: 'client123'. ";
        throw NonexistentRole("This role does not exist!");
    }
    else if (username == "collab") {
        std::cout << "You need to be more specific! Example: 'collab123'. ";
        throw NonexistentRole("This role does not exist!");
    }
    for (const auto &i : username) {
        if (isalpha(i)) roleName += i;  // TODO ai isto ta xanxo que fode e nem funciona bem
        else break;                     // TODO apagar este comentário
    }
    if (roleName == "collab") role = collaborator;
    else if (roleName == "client") role = client;
    else throw NonexistentRole("This role does not exist!");
    _role = role;
}

/**
 * @brief shows the client's operations
 * @param running
 */
void Interface::clientOperations(bool &running) {
    auto *client = dynamic_cast<Client *>(_user);
    bool innerRunning = true;
    Menu clientMenu("Welcome, " + client->getName(), {{"Request an Intervention",  [&]() {
        while (innerRunning) {
            std::string service = selectService(innerRunning);
            if (!service.empty()) {
                Date interventionDate = readInterventionDate();
                Service *sv = _houseMaster.getAvailableServices()[service];
                if (dynamic_cast<Painting *>(sv)) {
                    unsigned numberOfRooms = readNumberOfRooms();
                    client->requestIntervention(_houseMaster, interventionDate, service, false, numberOfRooms);
                } else {
                    client->requestIntervention(_houseMaster, interventionDate, service, false);
                }
            }
        }
    }}, {"Browse Services", [&]() {
        while (innerRunning) {
            std::string serviceName = selectService(innerRunning);
            if (!serviceName.empty()) {
                Service *service = _houseMaster.getAvailableServices()[serviceName];
                show(*service);
                std::cin.ignore();
            }
        }
    }},{"See active interventions", [&]() {
        while (innerRunning) {
            Intervention *intervention = selectActiveIntervention(innerRunning);
            if (intervention) {
                Menu activeInterventionMenu("Active intervention",{{"Mark as done",[&]() {
                    HouseMaster::markAsComplete(intervention);
                    showPayment(intervention);
                    std::cin.ignore();
                    _houseMaster.processTransaction(intervention);
                    Classification classification = readClassification(innerRunning);
                    _houseMaster.getCollaborators()[intervention->getCollabId()]->addClassification(classification);
                }},{"Cancel Intervention", [&]() {
                    Client::cancelIntervention(intervention);
                }}, {"See details", [&]() {
                    if (intervention->getService())
                        show(*intervention);
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

/**
 * @brief shows the collaborator's operations
 * @param running
 */
void Interface::collaboratorOperations(bool &running) {
    auto *collab = dynamic_cast<Collaborator *>(_user);
    bool innerRunning = true;
    Menu collabsMenu("Welcome, " + _user->getName(), {{"See profile",              [&]() {
        show(*collab);
        std::cin.ignore();
    }}, {"See active interventions", [&]() {
        Intervention *intervention = selectActiveIntervention(innerRunning);
        if (intervention) {
            Menu activeInterventionMenu("Active intervention",{{"See details", [&]() {
                if (!intervention->getService()->getName().empty())
                    show(*intervention);
                std::cin.ignore();
            }}});
            activeInterventionMenu.show();
            activeInterventionMenu.select();
            activeInterventionMenu.execute(running);
        }
    }},{"Learn a service", [&]() {
        Menu pickServices("Learn a service",{{"Choose from the HouseMaster services", [&]() {
            while (running) {
                std::string service = selectService(running);
                if (!service.empty()) {
                    if (!collab->canPreform(service))
                        collab->addService(service);
                    else
                        std::cout<< collab->getName()<< " already knows "<< service << "\n";
                }
            }
        }}, {"Add a new one", [&]() {
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

/**
 * @brief reads a new intervention's date
 * @return the date
 */
Date Interface::readInterventionDate() {
    std::string dateString{};
    std::cin.ignore();
    bool done;
    std::cout << "Insert the desired intervention date in format DD/MM/YYYY HH:mm\n";
    std::getline(std::cin, dateString);
    Date interventionDate;
    do {
        try {
            done = true;
            Date intDate(dateString);
            interventionDate = intDate;
            interventionDate.checkIfValid();
        } catch (const Date::InvalidDate &e) {
            done = false;
            std::cout << e.what() << "\nInsert the desired intervention date in format DD/MM/YYYY HH:mm\n";
            std::getline(std::cin, dateString);
            Date intDate(dateString);
            interventionDate = intDate;
        }
    } while (!done);
    return interventionDate;
}

/**
 * @brief selects a service from the housemaster catalogue
 * @param running
 * @return the service
 */
std::string Interface::selectService(bool &running) {
    auto services = _houseMaster.getAvailableServices();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : services) {
        options.insert(std::pair<std::string, std::function<void()>>(i.first, [&selection, &i]() {
            selection = i.first;
        }));
    }
    Menu servicesMenu("Select a service", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}

/**
 * @brief shows a service's details
 * @param service the service
 */
void Interface::show(const Service &service) {
    std::string pro = service.getPro() ? "yes" : "no";
    std::cout << " __________HOUSE MASTER__________ " << std::endl;
    std::cout << "| " << std::setw(30) << std::right << service.getName() << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [" << "Base Price" << "] " << std::setw(16) << std::right << service.getBasePrice() << "€ |"
              << std::endl;
    std::cout << "| [" << "Duration" << "] " << std::setw(19) << std::right << service.getDuration().getString() << " |"
              << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(15) << std::right << pro << " |" << std::endl;
    std::cout << "|                                |" << std::endl;
    std::cout << "| [Enter] Go Back                |" << std::endl;
    std::cout << "|________________________________|" << std::endl;
    std::cin.ignore();
}

/**
 * @brief shows admin's operations
 * @param running
 */
void Interface::adminOperations(bool &running) {

    bool innerRunning = true;
    Menu adminMenu("Welcome, ADMIN", {{"Register collaborator",           [&]() {
        readNewCollaboratorData(innerRunning);
    }}, {"Show all collaborators", [&]() {
        while (innerRunning) {
            std::string collabName = selectCollab(innerRunning);
            if (!collabName.empty()) {
                Collaborator *collab = _houseMaster.getCollaborators()[collabName];
                if (collab) show(*collab);
                std::cin.ignore();
            }
        }
    }},{"See HouseMaster finances", [&]() {
        showFinances();
        std::cin.ignore();
    }},{"Fire Collaborator", [&]() {
        std::string collabName = selectCollab(innerRunning);
        if (!collabName.empty()) {
            _houseMaster.removeCollaborator(collabName);
        }
    }},{"Show collaborators' performance", [&]() {
        showSortedCollabs();
        std::cin.ignore();
    }}});
    adminMenu.show();
    adminMenu.select();
    adminMenu.execute(running);
}

/**
 * @brief validates a nif
 * @param nif the nif
 * @return true if valid false otherwise
 */
bool Interface::isValidNif(unsigned nif) {
    if (nif / 1000000000 > 0) { throw InvalidNif("This is bigger than expected!"); }
    else if (nif / 100000000 != 1 && nif / 100000000 != 2 && nif / 100000000 != 5 && nif / 100000000 != 6 &&
             nif / 100000000 != 8 && nif / 100000000 != 9) { throw InvalidNif("This is an invalid NIF!"); }
    return true;
}

/**
 * @brief reads a new client's data
 */
void Interface::readNewClientData() {
    std::string name{}, premiumStr{};
    unsigned nif{};

    std::cout << "Name ? ";
    std::cin.ignore(9999, '\n');
    std::getline(std::cin, name, '\n');

    std::cout << "Premium ? [yes/no] ";
    std::cin >> premiumStr;
    while (premiumStr != "yes" && premiumStr != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> premiumStr;
    }

    bool premium = premiumStr == "yes";

    std::cout << "NIF ? ";
    std::cin >> nif; // TODO input validation
    while (std::cin.fail())
    {
        std::cin.clear();
        std::cin.ignore();
        std::cout << "This is an invalid NIF! Please try again:\nNIF ? ";
        std::cin >> nif;
    }
    bool done;
    do {
        try {
            done = true;
            isValidNif(nif);
        } catch (const InvalidNif &e) {
            done = false;
            std::cout << e.what() << "\nNIF ? ";
            std::cin >> nif;
        }
    } while (!done);

    _houseMaster.addClient(nif, name, premium);
    std::string username = (*_houseMaster.getClients().rbegin()).first;

    std::cout << "Welcome, " << name << " you can now login with the username " << username << "\n";

}

/**
 * @brief reads a new collaborator's data
 * @param running
 */
void Interface::readNewCollaboratorData(bool &running) {

    std::string name{}, pro{};
    std::vector<std::string> services{};

    std::cout << "Name ? ";
    std::cin.ignore(9999, '\n');
    std::getline(std::cin, name, '\n');

    std::cout << "Pro ? [yes/no] ";
    std::cin >> pro;
    while (pro != "yes" && pro != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> pro;
    }

    bool innerRunning = true;

    Menu pickServices("Pick your services", {{"Choose from the HouseMaster services", [&]() {
        while (running) {
            std::string service = selectService(running);
            if (!service.empty()) services.push_back(service);
        }
    }},{"Add a new one", [&]() {
        std::string serviceName = readNewServiceData(running);
        if (!serviceName.empty()) services.push_back(serviceName);
    }}});

    while (innerRunning) {
        pickServices.show();
        pickServices.select();
        pickServices.execute(innerRunning);
    }

    _houseMaster.addCollaborator(services, name, pro == "yes", 0);

}

/**
 * @brief reads a new service's data
 * @param running
 * @return the service's name
 */
std::string Interface::readNewServiceData(bool &running) {
    std::string name{}, proStr{}, durationStr{}, paintingStr{};
    float basePrice{};
    //duration duration{};

    std::cout << "Name ? ";
    std::cin.ignore();
    std::getline(std::cin, name, '\n');

    std::cout << "Pro ? [yes/no] ";
    std::cin >> proStr;
    while (proStr != "yes" && proStr != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> proStr;
    }

    bool pro = proStr == "yes";

    std::cout << "Base price ? ";
    std::cin >> basePrice;
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

    std::cout << "Mean duration ? ";
    std::cin >> durationStr;
    Duration duration(durationStr);

    std::cout << "Painting ? [yes/no] ";
    std::cin >> paintingStr;
    while (paintingStr != "yes" && paintingStr != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> paintingStr;
    }

    if (paintingStr == "yes") {
        _houseMaster.addAvailablePaintService(name, pro, basePrice, duration);
    } else {
        _houseMaster.addAvailableService(name, pro, basePrice, duration);
    }

    return name;

}

/**
 * @brief sellects a collab from housemaster
 * @param running
 * @return the collab's id
 */
std::string Interface::selectCollab(bool &running) {
    auto collabs = _houseMaster.getCollaborators();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : collabs) {
        options.insert(
                std::pair<std::string, std::function<void()>>(_houseMaster.getCollaborators()[i.first]->getName(), [&selection, &i]() {
                    selection = i.first;
                }));
    }
    Menu collabsMenu("Select a collaborator", options);
    collabsMenu.show();
    collabsMenu.select();
    collabsMenu.execute(running);
    return selection;
}

/**
 * @brief selects an active intervention for a user
 * @param running
 * @return the intervention
 */
Intervention *Interface::selectActiveIntervention(bool &running) {
    auto interventions = _houseMaster.getInterventions();
    std::vector<Intervention *> activeInterventions = _houseMaster.getAssociatedActiveInterventions(_user->getId());
    std::map<std::string, std::function<void()>> options{};
    Intervention *selection{};
    for (const auto &i : activeInterventions) {
        options.insert(std::pair<std::string, std::function<void()>>(i->getService()->getName() + " " +
        i->getStartingTime().getString(),[&selection, &i]() {selection = i;}));
    }
    Menu activeInterventionsMenu("Select an intervention", options);
    activeInterventionsMenu.show();
    activeInterventionsMenu.select();
    activeInterventionsMenu.execute(running);
    return selection;
}

/**
 * @brief shows a collaborator's profile
 * @param collaborator the collaborator
 */
void Interface::show(const Collaborator &collaborator) {
    std::string pro = collaborator.isPro() ? "yes" : "no";
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << collaborator.getName() << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [" << "ID" << "] " << std::setw(45) << std::right << collaborator.getId() << " |" << std::endl;
    std::cout << "| [" << "Score" << "] " << std::setw(42) << std::right << collaborator.getScore() << " |"
              << std::endl;
    std::cout << "| [" << "Professional" << "] " << std::setw(35) << std::right << pro << " |" << std::endl;
    std::cout << "| [" << "Earnings" << "] " << std::setw(39) << std::right << collaborator.getEarnings() << " |"
              << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Go Back                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}

/**
 * @brief shows an intervention's details
 * @param intervention the intervention
 */
void Interface::show(Intervention &intervention) {
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(49) << std::right << intervention.getService()->getName() << " |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "| [" << "Starting at" << "] " << std::setw(36) << std::right
              << intervention.getStartingTime().getString() << " |" << std::endl;
    std::cout << "| [" << "Ending at" << "] " << std::setw(38) << std::right << intervention.getEndTime().getString()
              << " |" << std::endl;
    std::cout << "| [" << "Cost" << "] " << std::setw(40) << std::right << intervention.getCost() << " |" << std::endl;
    std::cout << "| [" << "Collaborator" << "] " << std::setw(36) << std::right
              << _houseMaster.getCollaborators()[intervention.getCollabId()]->getName() << " |" << std::endl;
    std::cout << "|                                                     |" << std::endl;
    std::cout << "| [Enter] Go Back                                     |" << std::endl;
    std::cout << "|_____________________________________________________|" << std::endl;
    std::cin.ignore();
}

/**
 * @brief shows an invoice with payment details
 * @param intervention the intervention
 */
void Interface::showPayment(Intervention *intervention) {
    float cost = intervention->getCost();
    auto *client = dynamic_cast<Client *> (_user);
    std::cout << " ___________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(49) << std::right << intervention->getService()->getName() << " |" << std::endl;
    std::cout << "|                                                   |" << std::endl;
    std::cout << "| [" << "Client Name" << "] " << std::setw(35) << std::right << client->getName() << " |"
              << std::endl;
    std::cout << "| [" << "Client NIF" << "] " << std::setw(36) << std::right << client->getNif() << " |" << std::endl;
    std::cout << "| [" << "Total cost" << "] " << std::setw(36) << std::right << cost << " |" << std::endl;
    std::cout << "|                                                   |" << std::endl;
    std::cout << "| [Enter] Confirm                                   |" << std::endl;
    std::cout << "|___________________________________________________|" << std::endl;
    std::cin.ignore();
}

/**
 * @brief reads a classification
 * @param running
 * @return the classification
 */
Classification Interface::readClassification(bool &running) {
    Classification classification{};
    Menu classifications("Review intervention", {{"Unreliable",   [&classification]() {
        classification = unreliable;
    }},{"Clumsy", [&classification]() {
        classification = clumsy;
    }},{"Gets it done", [&classification]() {
        classification = getsItDone;
    }},{"Hardworking",  [&classification]() {
        classification = hardWorking;
    }},{"Attentive", [&classification]() {
        classification = attentive;
    }},{"Savior", [&classification]() {
        classification = savior;
    }}});
    classifications.show();
    classifications.select();
    classifications.execute(running);
    return classification;
}

/**
 * @brief getter
 * @return housemaster
 */
HouseMaster Interface::getHouseMasterState() const {
    return _houseMaster;
}

/**
 * @brief shows finances
 */
void Interface::showFinances() const {
    float money = _houseMaster.getEarnings();
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << "HouseMaster finances" << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [" << "Current Balance" << "] " << std::setw(32) << std::right << money << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Confirm                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}

/**
 * @brief reads number of rooms
 * @return the number of rooms
 */
unsigned Interface::readNumberOfRooms() {
    unsigned rooms{};
    std::cout << "How many rooms to paint?\n";
    std::cin >> rooms;
    // TODO input validation eewwwwww
    return rooms;
}

/**
 * @brief shows collaborator's rankings
 */
void Interface::showSortedCollabs() {
    std::vector<std::pair<std::string, Collaborator *>> sorted = _houseMaster.sortCollaboratorsByScore();
    unsigned long back = sorted.size() + 1;

    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << "Collaborators" << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    for (const auto &collab: sorted) {
        std::cout << "| " << std::left << std::setw(48) << collab.second->getName() << std::setw(2)
                  << collab.second->getScore() << " |" << std::endl;
    }
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [0] EXIT (no save)" << "                      [" << std::setw(2) << std::right << back
              << "] BACK  |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}
