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
