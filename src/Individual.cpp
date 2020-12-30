#include "Individual.h"

#include <utility>


unsigned int Collaborator::_idSeqCol = 0;
unsigned int Client::_idSeqClt = 0;
unsigned int Admin::_idSeqAdmins = 0;

/**
 * @brief Individual constructor
 */
Individual::Individual() : _displayName(), _id() {}


/**
 * @brief Individual constructor
 * @param name individual's name
 */
Individual::Individual(std::string name) : _displayName(std::move(name)), _id(0) {
}


/**
 * @brief getter
 * @return the individual's name
 */
std::string Individual::getName() const {
    return _displayName;
}


// Collaborator associated methods

/**
 * @brief Collaborator's constructor
 * @param services the services the collaborator can preform
 * @param name the collaborator's name
 * @param pro is professional
 * @param earnings the earnings
 * @param score the score
 * @param affiliate the affiliate's name
 */
Collaborator::Collaborator(std::vector<std::string> services, const std::string &name, bool pro,
                           std::vector<Availability> &availability, float earnings, Classification score,
                           std::string affiliate) : Individual(name),
                                                   _services(std::move(services)), _score(score), _pro(pro),
                                                   _earnings(earnings), _affiliate(std::move(affiliate)) {
    _id = _idSeqCol++;

    std::sort(availability.begin(), availability.end());
    _availability = availability;
}

/**
 * @brief exception in case someone wants to add a service already known by the collaborator
 * @param error_msg to show
 */
Collaborator::AlreadyKnows::AlreadyKnows(const std::string &error_msg): std::logic_error(error_msg){};

/**
 * @brief exception for a non professional collaborator wanting to learn a service that requires a professional
 * @param error_msg to show
 */
Collaborator::ServiceRequiresPro::ServiceRequiresPro(const std::string &error_msg): std::logic_error(error_msg){};


/**
 * @brief getter
 * @return the services
 */
std::vector<std::string> Collaborator::getServices() {
    return _services;
}

/**
 * @brief getter
 * @return true if professional false otherwise
 */
bool Collaborator::isPro() const {
    return _pro;
}

/**
 * @brief checks if a collaborator can preform a service
 * @param service the service to check
 * @return true if can preform false otherwise
 */
bool Collaborator::canPreform(const std::string &service) {
    auto found = std::find_if(_services.begin(), _services.end(), [&service](const std::string &service1) {
        return service == service1;
    });
    return found != _services.end();
}

/**
 * @brief checks if a collaborator has the qualification to preform an intervention
 * @param intervention the intervention
 * @return true if the collaborator has qualifications false otherwise
 */
bool Collaborator::hasQualificationToPreform(Intervention *intervention) const {
    return !(intervention->getService()->getPro() || intervention->getForcePro()) || isPro();
}

/**
 * @brief getter
 * @return score
 */
int Collaborator::getScore() const {
    return _score;
}

/**
 * @brief getter
 * @return affiliate
 */
std::string Collaborator::getAffiliate() const
{
    return _affiliate;
}

/**
 * @brief add a classification to the collaborator
 * @param classification the classification
 */
void Collaborator::addClassification(Classification classification) {
    _classifications.push_back(classification);
    updateScore();
}

/**
 * @brief updates the collaborator's score
 */
void Collaborator::updateScore() {
    double sum{}, average{};
    if (_classifications.empty()) _score = newHere;
    else {
        std::for_each(_classifications.begin(), _classifications.end(), [&](int n) {
            sum += n;
        });
        average = (int(_score) + sum / static_cast<double>(_classifications.size())) / 2;
        _score = static_cast<Classification>(int(round(average)));
    }
}

/**
 * @brief calculates the collaborator's earnings
 * @param hmEarnings the total earnings
 */
void Collaborator::calculateEarnings(float &hmEarnings) {
    float collabEarnings = commissionByScore() * hmEarnings;
    _earnings += collabEarnings;
    hmEarnings = hmEarnings - collabEarnings;
}

/**
 * @brief getter
 * @return the collaborator's earnings
 */
float Collaborator::getEarnings() const {
    return _earnings;
}

/**
 * @brief gets the collaborator's commission according to their score
 * @return the value
 */
float Collaborator::commissionByScore() {
    if (_score < 1) return CommissionByScore_0;
    else if (_score < 2) return CommissionByScore_1;
    else if (_score < 3) return CommissionByScore_2;
    else if (_score < 4) return CommissionByScore_3;
    else return CommissionByScore_4;
}

/**
 * @brief getter
 * @return id
 */
std::string Collaborator::getId() const {
    std::stringstream outStr;
    outStr << "collab" << _id;
    return outStr.str();
}
 /** @brief Availability getter
  * @return availanylity vector
  */
const std::vector<Availability> &Collaborator::getAvailability() const {
    return _availability;
}

/**
 * @brief checks if two collaborators are the same
 * @param ind2 other collaborator
 * @return true if they're the same false otherwise
 */
bool Collaborator::operator==(const Collaborator &ind2) const {
    return _id == ind2._id;
}

/**
 * @brief checks if a collaborator has less score than other
 * @param col2 other collaborator
 * @return true if the score is less than the other false otherwise
 */
bool Collaborator::operator<(const Collaborator &col2) const {
    return _score < col2._score;
}

/**
 * @brief adds a service to the collaborator's services
 * @param service the service
 */
void Collaborator::addService(Service *service) {
    if (canPreform(service->getName())) throw AlreadyKnows("This collaborator already knows this service");
    if(!isPro() && service->getPro()) throw ServiceRequiresPro("This collaborator can't preform professional service");
    _services.push_back(service->getName());
}

// Client associated methods

/**
 * @brief client's constructor
 * @param nif tax id
 * @param name the name
 * @param affiliate the affiliate's name
 * @param premium is premium
 */
Client::Client(unsigned int nif, const std::string &name, bool premium, std::string affiliate)
        : Individual(name), _nif(nif), _premium(premium), _affiliate(std::move(affiliate)) {
    _id = _idSeqClt++;
    std::stringstream m{};
    m << "client" << _id << "@housemaster.pt";
    _email = m.str();
}

/**
 * @brief getter
 * @return nif
 */
unsigned int Client::getNif() {
    return (_nif);
}

/**
 * @brief getter
 * @return affiliate
 */
std::string Client::getAffiliate()
{
    return _affiliate;
}

/**
 * @brief getter
 * @return premium
 */
bool Client::isPremium() const {
    return _premium;
}

/**
 * @brief getter
 * @return id
 */
std::string Client::getId() const {
    std::stringstream outStr;
    outStr << "client" << _id;
    return outStr.str();
}

/**
 * @brief compares two clients
 * @param ind2 the other client
 * @return true if they're the same false otherwise
 */
bool Client::operator==(const Client &ind2) const {
    return _id == ind2._id;
}

std::string Client::getEmail() const {
    return _email;
}

void Client::setEmail(const std::string &email) {
    _email = email;
}


/**
 * @brief admin's constructor
 * */
Admin::Admin() : Individual() {};


/**
 * @brief admin's constructor
 * @param name the name
 * @param affiliate the affiliate
 */
Admin::Admin(const std::string &name) : Individual(name) {
    _id = _idSeqAdmins++;
}

/**
 * @brief getter
 * @return id
 */
std::string Admin::getId() const {
    std::stringstream outStr;
    outStr << "admin" << _id;
    return outStr.str();
}