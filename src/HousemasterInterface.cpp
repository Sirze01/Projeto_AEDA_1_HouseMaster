//
// Created by ritam on 27/12/20.
//

#include "HousemasterInterface.h"

HousemasterInterface::HousemasterInterface(const HouseMaster &housemaster) : _currentAffiliate(), _houseMaster(housemaster) {

}

void HousemasterInterface::showAffiliateInterface(HouseMasterAffiliate &affiliate) {
    bool running = true;
    Interface affiliateInterface(affiliate);
    while (running) {
        affiliateInterface.selectRole(running);
    }
    _houseMaster.removeAffiliate(affiliate);
    _houseMaster.registerAffiliate(affiliateInterface.getHouseMasterState());
}

HouseMasterAffiliate HousemasterInterface::selectAffiliate(bool &running) {
    std::map<std::string, std::function<void()> > options{};
    auto affiliates = _houseMaster.getAffiliates();
    HouseMasterAffiliate selection{};
    BSTItrIn<HouseMasterAffiliate> current(affiliates);
    std::set<HouseMasterAffiliate> hms{};
    for (; !current.isAtEnd(); current.advance()) {
        std::cout << "Inserting " << current.retrieve().getAffiliateName() << "\n";
        hms.insert(current.retrieve());
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

void HousemasterInterface::runAffiliateInterface(bool &running) {
    while (running) {
        HouseMasterAffiliate affiliate = selectAffiliate(running);
        std::cout << "Showing " << affiliate.getAffiliateName() << "\n";
        showAffiliateInterface(affiliate);
    }
}

void HousemasterInterface::showInterface(bool &running) {
    Menu start("Welcome to Housemaster", {{"Show Finances", [&](){
        std::cout << _houseMaster.getTotalFinances() << "\n";
    }},{"Filter by location", [&](){
        std::string location = selectLocation(running);
        std::vector<HouseMasterAffiliate> hms = _houseMaster.getAffiliatesByLocation(location);
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
    }}, {"Filter by responsible", [&](){
        std::string responsible = selectResponsible(running);
        std::vector<HouseMasterAffiliate> hms = _houseMaster.getAffiliatesByResponsible(responsible);
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
    }}});
    start.show();
    start.select();
    start.execute(running);
}

std::string HousemasterInterface::selectLocation(bool &running) {
    auto location = _houseMaster.getLocations();
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
    auto responsible = _houseMaster.getResponsibles();
    std::string selection{};
    std::map<std::string, std::function<void()>> options{};
    for (const auto &i : responsible) {
        options.insert(std::pair<std::string, std::function<void()>>(i, [&selection, &i]() {
            selection = i;
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
