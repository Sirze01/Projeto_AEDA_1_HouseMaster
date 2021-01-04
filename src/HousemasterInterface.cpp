#include "HousemasterInterface.h"


HousemasterInterface::HousemasterInterface(HouseMaster *housemaster) : _currentAffiliate(), _houseMaster(housemaster) {

}

HouseMasterAffiliate HousemasterInterface::selectAffiliate(bool &running) {
    std::map<std::string, std::function<void()> > options{};
    auto affiliates = _houseMaster->getAffiliates();
    HouseMasterAffiliate selection{};
    BSTItrIn<HouseMasterAffiliate> current(affiliates);
    std::set<HouseMasterAffiliate> hms{};
    for (; !current.isAtEnd(); current.advance()) {
        hms.insert(current.retrieve());
    }
    for (const auto & i : hms) {
        options.insert(std::pair<std::string, std::function<void()>>(i.getLocation(),[&](){
            selection = i;
        }));
    }
    Menu affiliateSelection("Select your affiliate", options);
    affiliateSelection.show();
    affiliateSelection.select();
    affiliateSelection.execute(running);
    return selection;
}

HouseMasterAffiliate HousemasterInterface::selectResponsibleAffiliate(bool &running) {
    std::map<std::string, std::function<void()> > options{};
    BST<HouseMasterAffiliate> affiliates = _houseMaster->getAffiliates();
    HouseMasterAffiliate selection{};
    BSTItrIn<HouseMasterAffiliate> current(affiliates);
    std::set<HouseMasterAffiliate> hms{};
    auto admin = dynamic_cast<Admin*>(_user);
    for (; !current.isAtEnd(); current.advance()) {
        std::string location = current.retrieve().getLocation();
        std::string adminId = current.retrieve().getAdmin().getId();
        if (adminId == admin->getId()) {
            for (const auto &i : admin->getAffiliates()) {
                if (i == location) hms.insert(current.retrieve());
            }
        }
    }
    for (const auto & i : hms) {
        options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(),[&](){
            selection = i;
        }));
    }
    Menu affiliateSelection("Select your affiliate", options);
    affiliateSelection.show();
    affiliateSelection.select();
    affiliateSelection.execute(running);
    return selection;
}




void HousemasterInterface::firstInterface(bool &running) {
    bool innerRunning = true;
    Menu start("Welcome to Housemaster", {{"HouseMaster Administration", [&](){
        std::cout << "Welcome to HouseMaster. You have ADMIN privilege.\n";
        adminLogin();
        auto toChange = _currentAffiliate;
        housemasterOperations(running);
        while (innerRunning) {
            start.show();
            start.select();
            start.execute(innerRunning);
            std::cin.ignore();
        }
        _houseMaster->removeAffiliate(toChange);
        _houseMaster->registerAffiliate(_currentAffiliate);
    }},{"Login Client", [&](){
        clientLogin();
        Interface clientInterface(_houseMaster, _currentAffiliate, _user, client);
        while (innerRunning) {
            clientInterface.clientOperations(innerRunning);
        }
        *_houseMaster = clientInterface.getHousemasterState();
        _houseMaster->removeAffiliate(_currentAffiliate);
        _houseMaster->registerAffiliate(clientInterface.getHousemasterAffiliateState());
    }}, {"Login Collaborator", [&](){
        collabLogin();
        Interface collabInterface(_houseMaster, _currentAffiliate, _user, collaborator);
        std::cout << "Login succeeded for " << _user->getName() << "\n";
        while (innerRunning) {
            collabInterface.collaboratorOperations(innerRunning);
        }
        *_houseMaster = collabInterface.getHousemasterState();
        _houseMaster->removeAffiliate(_currentAffiliate);
        _houseMaster->registerAffiliate(collabInterface.getHousemasterAffiliateState());

        std::cout << ".....\n";
    }}, { "Login Responsible", [&](){
        std::cout << "Welcome to HouseMaster. You have RESPONSIBLE privilege.\n";
        std::string responsibleId = selectResponsible(innerRunning);
        responsibleLogin(responsibleId);
        _currentAffiliate = selectResponsibleAffiliate(innerRunning);
        _currentAffiliate.setHousemaster(_houseMaster);
        std::cout << "Affiliate " << _currentAffiliate.getAffiliateName() << "\n";
        Interface adminInterface(_houseMaster, _currentAffiliate, _user, admin);
        while (innerRunning) {
            adminInterface.responsibleOperations(innerRunning);
        }
        *_houseMaster = adminInterface.getHousemasterState();
        if (!_currentAffiliate.getAffiliateName().empty()) {
            _houseMaster->removeAffiliate(_currentAffiliate);
            _houseMaster->registerAffiliate(adminInterface.getHousemasterAffiliateState());
        }
      }},{"Register Client", [&]() {
        readNewClientData();
      }}});
    start.show();
    start.select();
    start.execute(running);
}

/**
 * @brief reads a new client's data
 */
void HousemasterInterface::readNewClientData() {
    std::string name{}, email{}, premiumStr{};
    unsigned nif{};

    std::cin.ignore(9999, '\n');

    std::cout << "Name ? ";
    std::getline(std::cin, name, '\n');

    std::cout << "E-mail ? ";
    std::getline(std::cin, email, '\n');

    std::cout << "Premium ? [yes/no] ";
    std::cin >> premiumStr;
    while (premiumStr != "yes" && premiumStr != "no") {
        std::cout << R"(Invalid choice. Make sure you chose one of "yes" or "no" )" << std::endl;
        std::cin >> premiumStr;
    }

    bool premium = premiumStr == "yes";

    std::cout << "NIF ? ";
    std::cin >> nif;
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
            Interface::isValidNif(nif);
        } catch (const Interface::InvalidNif &e) {
            done = false;
            std::cout << e.what() << "\nNIF ? ";
            std::cin >> nif;
        }
    } while (!done);
    bool running = true;
    HouseMasterAffiliate affiliate = selectAffiliate(running);
    try
    {
        _houseMaster->addClient(nif, name, email, premium, affiliate.getAffiliateName());
    } catch (HouseMaster::ExistentClient &e)
    {
        std::cout << e.what() << std::endl;
    }
}


/**
 * @brief admin login
 */
void HousemasterInterface::adminLogin() {
    std::string password{};
    std::cout << "Password : ";
    std::cin >> password;
    for (int i = 0; i <= 1; i++) {
        if (password == "admin") {
            std::cout << "success \n";
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
 * @brief responsible login
 */
void HousemasterInterface::responsibleLogin(const std::string& responsibleId) {
    std::string password{};
    std::cout << "Password : ";
    std::cin >> password;
    _user = _houseMaster->getAdmins().find(responsibleId)->second;
    for (int i = 0; i <= 1; i++) {
        if (password == dynamic_cast<Admin*>(_user)->getPassword()) return;
        std::cout << "Wrong password. Try again:\n";
        std::cin >> password;
    }
    std::cout << "Too many tries! Logging out...\n";
    exit(0);
}




/**
 * @brief collab login
 */
void HousemasterInterface::collabLogin() {
    std::string username{};
    std::cout << "Username: ";
    std::cin.ignore();
    std::getline(std::cin, username);
    bool done = false;
    while (!done) {
        try{
            done = true;
            _user = _houseMaster->findByUsername(username);
            _currentAffiliate = _houseMaster->findAffiliateByCollab(dynamic_cast<Collaborator*>(_user));
        }
        catch (const HouseMaster::NonexistentUsername &e) {
            done = false;
            std::cout << e.what() << " Please try again\n";
            std::cout << "Username : ";
            std::cin >> username;
        }
    }
}

/**
 * @brief client login
 */

void HousemasterInterface::clientLogin() {
    std::string email{};
    std::cout << "E-mail: ";
    std::cin.ignore();
    std::getline(std::cin, email);
    bool done = false;
    while (!done) {
        try {
            done = true;
            Client* client = _houseMaster->findClientByEmail(email);
            _currentAffiliate = _houseMaster->findAffiliateByClient(client);
            _currentAffiliate.setHousemaster(_houseMaster);
            _user = client;
        }
        catch (const HouseMaster::NonexistentClient &e) {
            done = false;
            std::cout << e.what() << " Please try again\n";
            std::cout << "E-mail: ";
            std::cin >> email;
        }
    }
}



void HousemasterInterface::housemasterOperations(bool &running) {
    Menu start("Welcome to Housemaster", {{"Show Finances", [&](){
        showTotalFinances(*_houseMaster);
        std::cin.ignore();
    }},{"Filter by location", [&](){
        bool innerRunning = true;
        std::string location = selectLocation(running);
        std::vector<HouseMasterAffiliate> hms = _houseMaster->getAffiliatesByLocation(location);
        HouseMasterAffiliate selection{};
        std::map<std::string, std::function<void()>> options{};
        for (const auto &i : hms) {
            options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(), [&]() {
                show(i);
            }));
        }
        Menu affiliates("See details", options);
        while (innerRunning) {
            affiliates.show();
            affiliates.select();
            affiliates.execute(innerRunning);
            std::cin.ignore();
        }
    }}, {"Filter by responsible", [&](){
        bool innerRunning = true;
        std::string responsible = selectResponsible(innerRunning);
        std::vector<HouseMasterAffiliate> hms = _houseMaster->getAffiliatesByResponsible(responsible);
        HouseMasterAffiliate selection{};
        std::map<std::string, std::function<void()>> options{};
        for (const auto &i : hms) {
            options.insert(std::pair<std::string, std::function<void()>>(i.getAffiliateName(), [&]() {
                show(i);
            }));
        }
        Menu affiliates("See details", options);
        affiliates.show();
        affiliates.select();
        affiliates.execute(running);
        std::cin.ignore();
    }},{ "Register affiliate",[&](){
     readNewAffiliateData();
    }},{"Remove affiliate", [&](){
        bool innerRunning = true;
        HouseMasterAffiliate affiliate = selectAffiliate(innerRunning);
        _houseMaster->removeAffiliate(affiliate);
    }}});
    start.show();
    start.select();
    start.execute(running);
}


std::string HousemasterInterface::selectLocation(bool &running) {
    auto location = _houseMaster->getLocations();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : location) {
        options.insert(std::pair<std::string, std::function<void()>>(i, [&selection, &i]() {
            selection = i;
        }));
    }
    Menu servicesMenu("Select a Location", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}


std::string HousemasterInterface::selectResponsible(bool &running) {
    auto responsible = _houseMaster->getResponsibles();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : responsible) {
        options.insert(std::pair<std::string, std::function<void()>>(i.second->getName(), [&selection, &i]() {
            selection = i.first;
        }));
    }
    Menu servicesMenu("Select a Responsible", options);
    servicesMenu.show();
    servicesMenu.select();
    servicesMenu.execute(running);
    return selection;
}


void HousemasterInterface::show(const HouseMasterAffiliate& affiliate) {
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "| " << std::setw(50) << std::right << affiliate.getAffiliateName() << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [" << "Location" << "] " << std::setw(39) << std::right << affiliate.getLocation() << " |" << std::endl;
    std::cout << "| [" << "Responsible" << "] " << std::setw(36) << std::right << affiliate.getAdmin().getName() << " |"
              << std::endl;
    std::cout << "| [" << "Earnings" << "] " << std::setw(39) << std::right << affiliate.getEarnings() << " |"
              << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Go Back                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}

void HousemasterInterface::showTotalFinances(const HouseMaster &hm){
    std::cout << " ____________________HOUSE MASTER____________________ " << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "|  " << "Total" << std::setw(46) << "|" << std::endl;
    std::cout << "| " << " Finances"  << std::setw(41) << std::right << hm.getTotalFinances() << " |" << std::endl;
    std::cout << "|                                                    |" << std::endl;
    std::cout << "| [Enter] Go Back                                    |" << std::endl;
    std::cout << "|____________________________________________________|" << std::endl;
    std::cin.ignore();
}


/**
 * @brief reads a new client's data
 */
void HousemasterInterface::readNewAffiliateData() {
    std::string name{}, location{};

    std::cout << "Affiliate's Name? ";
    std::cin.ignore(9999, '\n');
    std::getline(std::cin, name, '\n');

    std::cout << "Affiliate's Location? ";
    std::cin.ignore(9999, '\n');
    std::getline(std::cin, location, '\n');

    /*try
    {*/
    _houseMaster->registerAffiliate(HouseMasterAffiliate(_houseMaster,location, name));
    /*} catch (HouseMasterAffiliate::ExistentAffiliate &e) //TODO ExistentAffiliate
    {
        std::cout << e.what() << std::endl;
    }*/
}