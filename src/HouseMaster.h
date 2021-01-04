#ifndef SRC_HOUSEMASTER_H
#define SRC_HOUSEMASTER_H


#include <iostream>
#include "bst.h"
#include "HouseMasterAffiliate.h"

/** @brief auxiliary struct to hash clients based on their email address */
struct ClientHash {
    /** @brief client hash function based on the client email*/
    int operator()(const Client *client) const {
        return std::hash<std::string>()(client->getEmail());
    }
    /** @brief client equality operator to detect collisions*/
    bool operator()(const Client *c1, const Client *c2) const {
        return c1->getEmail() == c2->getEmail();
    }
};

typedef std::unordered_set<Client*, ClientHash, ClientHash> clientHT;

/**
 * @brief This class keeps the various housemaster affiliates
 */
class HouseMaster {
public:
    // Constructors and destructor
    HouseMaster();

    explicit HouseMaster(std::ifstream affiliates);

    ~HouseMaster() = default;


    // Affiliates manipulation
    BST<HouseMasterAffiliate >& getAffiliates();

    void registerAffiliate(const HouseMasterAffiliate& affiliate);

    void removeAffiliate(const HouseMasterAffiliate& affiliate);

    vector<HouseMasterAffiliate> getAffiliatesByLocation(const string& location);

    vector<HouseMasterAffiliate> getAffiliatesByResponsible(const string& /*(ou outra coisa nsei)*/ responsible);

    std::set<std::string> getLocations() {return _locations;}

    std::map<std::string, Admin *> getResponsibles(){return _responsibles;};

    HouseMasterAffiliate findAffiliateByClient(const Client *client) const;

    HouseMasterAffiliate findAffiliateByCollab(const Collaborator *collab) const;

    void writeAffiliatesInfo();


    // Users Manipulation
    Individual *findByUsername(const std::string &name);

    std::map<std::string, Client *> getClients() const;

    void addClient(unsigned long nif, const std::string &name, std::string email, bool premium, std::string affiliate);

    void removeClient(const std::string &clientId);

    Client * findClientByEmail(const std::string &email) const;

    void changeClientEmail(const std::string& oldEmail, const std::string& newEmail);

    clientHT getContacts() const;

    void addUsernamesMapEntry(std::pair<std::string, std::string> map);

    std::map<std::string, Collaborator *> getCollaborators() const;

    void removeCollaborator(const std::string &id); // Don't use, use affiliate instead

    void addCollaborator(const std::vector<std::string> &services, const std::string &name, bool pro,
                         std::vector<Availability> availabilities, float earnings,
                         Classification score, std::string affiliate);

    Collaborator* findCollabById(const std::string &collabId);

    std::map<std::string, Admin *> getAdmins() const {return _responsibles;};

    void addAdmin(const std::string &name, std::string password, const std::vector<std::string> &affiliates);

    void usernameMapChanger(std::string id, std::string newUsername);

    void writeUsernameMap();

    // General
    float getTotalFinances() const;

    // Exceptions
    class UsernameAlreadyInUse;

    class NonexistentUsername;

    class NonexistentCollab;

    class ExistentClient;

    class NonexistentClient;

    class NonexistentResponsible;

    class UnableToWriteFile;

private:
    std::unordered_map<std::string, std::string> _usernameMap;
    std::map<std::string, Client *> _clients;
    std::map<std::string, Collaborator *> _collaborators;
    std::set<std::string> _locations;
    std::map<std::string, Admin *> _responsibles;
    BST<HouseMasterAffiliate> _affiliates;
    clientHT _clientContacts;

};

// Exceptions
class HouseMaster::UsernameAlreadyInUse : public std::logic_error{
public:
    explicit UsernameAlreadyInUse(const std::string &error_msg);
};

class HouseMaster::NonexistentUsername : public std::out_of_range {
public:
    explicit NonexistentUsername(const std::string &error_msg);
};

class HouseMaster::NonexistentCollab : public std::out_of_range {
public:
    explicit NonexistentCollab(const std::string &error_msg);
};

class HouseMaster::NonexistentResponsible : public std::out_of_range {
public:
    explicit NonexistentResponsible(const std::string &error_msg);
};

class HouseMaster::ExistentClient : public std::out_of_range {
public:
    explicit ExistentClient(const std::string &error_msg);
};

class HouseMaster::NonexistentClient : public std::out_of_range {
public:
    explicit NonexistentClient(const std::string &error_msg);
};

class HouseMaster::UnableToWriteFile : public std::ios_base::failure {
public:
    explicit UnableToWriteFile(const std::string &error_msg);
};




#endif //SRC_HOUSEMASTER_H
