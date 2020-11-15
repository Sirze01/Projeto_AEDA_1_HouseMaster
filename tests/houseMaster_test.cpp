#include <gtest/gtest.h>
#include "../src/HouseMaster.h"
#include <sstream>
#include <unordered_map>
using testing::Eq;


TEST(HouseMasterTest, fileConstructor){
    std::stringstream out;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    std::vector<Service> svc;
    svc.emplace_back("ajudar os amiguinhos com fisica", false, 3, duration(2,30));
    svc.emplace_back("ajudar os amiguinhos com amat", false, 3, duration(3,0));
    svc.emplace_back("jantar na cantina as 18h30", false, 2.70, duration(1,0));
    svc.emplace_back("montar um carro do nada", false, 2.5, duration(10,0));
    svc.emplace_back("desmontar um computador", false, 4, duration(2,0));
    svc.emplace_back("canalizador", true, 5, duration(3,0));
    svc.emplace_back("andar a correr atras de uma princesa ou la o que e", false, 1, duration(4,0));
    for(auto & i : svc){
        EXPECT_EQ(i.getName(), houseMaster1.getAvailableServices()[i.getName()]->getName());
        EXPECT_EQ(i.getPro(), houseMaster1.getAvailableServices()[i.getName()]->getPro());
        EXPECT_EQ(i.getDuration(), houseMaster1.getAvailableServices()[i.getName()]->getDuration());
        EXPECT_EQ(i.getBasePrice(), houseMaster1.getAvailableServices()[i.getName()]->getBasePrice());
    }


    Client::_idSeqClt= 0 ;
    std::vector<Client> cltVec;
    cltVec.emplace_back(22331122, "sonso", false);
    cltVec.emplace_back(453455323, "socorro", false);
    cltVec.emplace_back(23421234, "aaaaaaa", true);
    cltVec.emplace_back(323432234, "cliente de teste", false);

    for(auto &client:cltVec){
        out.str("");
        out << client.getId();

        EXPECT_EQ(client.getNif(), houseMaster1.getClients()[out.str()]->getNif());
        EXPECT_EQ(client.getName(), houseMaster1.getClients()[out.str()]->getName());
    }



    Collaborator::_idSeqCol = 0;
    std::vector<Collaborator> colVec;
    std::vector<std::string> tmpvec;
    tmpvec.emplace_back("ajudar os amiguinhos com fisica");
    tmpvec.emplace_back("ajudar os amiguinhos com amat");
    tmpvec.emplace_back("jantar na cantina as 18h30");
    colVec.emplace_back(tmpvec, "Miro Osvaldo", true);
    tmpvec.clear();
    tmpvec.emplace_back("montar um carro do nada");
    tmpvec.emplace_back("desmontar um computador");
    colVec.emplace_back(tmpvec, "Jose Retiros", false);
    tmpvec.clear();
    tmpvec.emplace_back("canalizador");
    tmpvec.emplace_back("andar a correr atras de uma princesa ou la o que e");
    colVec.emplace_back(tmpvec, "Super Mario", true);

    for(auto &collaborator : colVec){
        out.str("");
        out << collaborator.getId();

        EXPECT_EQ(collaborator.getName(), houseMaster1.getCollaborators()[out.str()]->getName());
        EXPECT_EQ(collaborator.isPro(), houseMaster1.getCollaborators()[out.str()]->isPro());
        EXPECT_EQ(collaborator.getServices(), houseMaster1.getCollaborators()[out.str()]->getServices());
        EXPECT_EQ(collaborator.getScore(), houseMaster1.getCollaborators()[out.str()]->getScore());

    }

}


TEST(HouseMasterTest, servicesManip){
    Collaborator::_idSeqCol = 0;
    Client::_idSeqClt = 0;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));


    // remove service
    houseMaster1.removeAvailableService("ajudar os amiguinhos com fisica");
    EXPECT_EQ(houseMaster1.getAvailableServices().find("ajudar os amiguinhos com fisica"), houseMaster1.getAvailableServices().end());

    // Try to remove inexistent service
    EXPECT_THROW(houseMaster1.removeAvailableService("ajudar os amiguinhos com fisica"),HouseMaster::InexistentService);

    // add service
    houseMaster1.addAvailableService("ajudar os amiguinhos com fisica", false, 3, duration(2,30));
    EXPECT_FALSE(houseMaster1.getAvailableServices().find("ajudar os amiguinhos com fisica") == houseMaster1.getAvailableServices().end());

    // Try to add existent service
    EXPECT_THROW(houseMaster1.addAvailableService("ajudar os amiguinhos com fisica", false, 3, duration(2,30)),HouseMaster::ExistentService);
}


TEST(HouseMasterTest, collaboratorsManip){
    Collaborator::_idSeqCol = 0;
    Client::_idSeqClt = 0;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));


    // remove collaborator
    houseMaster1.removeCollaborator("collab1");
    EXPECT_EQ(houseMaster1.getCollaborators().find("collab1"), houseMaster1.getCollaborators().end());

    // remove already removed collaborator
    EXPECT_THROW(houseMaster1.removeCollaborator("collab1"), HouseMaster::InexistentCollab);

    // add collaborator
    std::vector<std::string> functions;
    functions.push_back("montar um carro do nada"); functions.push_back("desmontar um computador");
    houseMaster1.addCollaborator(functions, "Jose Retiros", false);
    EXPECT_FALSE(houseMaster1.getCollaborators().find("collab3") == houseMaster1.getCollaborators().end());

}


TEST(HouseMasterTest, clientsManip){
    Collaborator::_idSeqCol = 0;
    Client::_idSeqClt = 0;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    // remove client
    houseMaster1.removeClient("client0");
    EXPECT_EQ(houseMaster1.getClients().find("client0"), houseMaster1.getClients().end());
    EXPECT_THROW(houseMaster1.removeClient("client0"), HouseMaster::InexistentClient);

    // add client
    houseMaster1.addClient(1111232, "mafarrico", false);
    EXPECT_FALSE(houseMaster1.getClients().find("client4") == houseMaster1.getClients().end());
    EXPECT_THROW(houseMaster1.addClient(1111232, "mafarrico", false), HouseMaster::ExistentClient);

}


TEST(HouseMasterTest, interventionManip){
    Collaborator::_idSeqCol = 0;
    Client::_idSeqClt = 0;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    // add Intervention
    houseMaster1.addIntervention(date(12,05,2001,01,00), "desmontar um computador", true, "");
    auto it =  std::find_if(houseMaster1.getInterventions().begin(), houseMaster1.getInterventions().end(), [](Intervention * intervention){
        if(intervention->getClientId() == "" && intervention->getService()->getName() == "desmontar um computador"){return true;}});
    EXPECT_FALSE(it == houseMaster1.getInterventions().end());

    houseMaster1.getClients()["client3"]->requestIntervention(houseMaster1, date(12, 12, 2012, 21, 00), "canalizador",
                                                              false);
    it = std::find_if(houseMaster1.getInterventions().begin(), houseMaster1.getInterventions().end(), [](Intervention * intervention){
        if(intervention->getClientId() == "client3" && intervention->getService()->getName() == "canalizador" && *intervention->getStartingTime() == date(12,12,2012,21,00)){return true;}});
    EXPECT_FALSE(it == houseMaster1.getInterventions().end());

}


TEST(HouseMasterTest, usageTest){
    Collaborator::_idSeqCol = 0;
    Client::_idSeqClt = 0;
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    // Client requests intervention

    houseMaster1.getClients()["client3"]->requestIntervention(houseMaster1,
                                                              date(12, 11, 2020, 12, 00), "desmontar um computador",
                                                              false);

    // Client cancels it
    houseMaster1.getClients()["client3"]->cancelIntervention(houseMaster1.getClients()["client3"]->getAssociatedInterventions(houseMaster1).at(0));

    // client requests another
    houseMaster1.getClients()["client2"]->requestIntervention(houseMaster1,
                                                              date(12, 11, 2020, 12, 00),
                                                              "andar a correr atras de uma princesa ou la o que e",
                                                              false);

    // Admin assigns collab
    houseMaster1.assignColaborator(houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0), houseMaster1.sortCollaboratorsByScore());

    // Collab starts working
    houseMaster1.getCollaborators()
    [houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0)->getCollabId()]->
    markInterventionAsInProgress(houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0));

    // Collab concludes
    houseMaster1.getCollaborators()
    [houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0)->getCollabId()]->
            markInterventionAsComplete(houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0));

    // Client classifies collaborator
    houseMaster1.getClients()["client2"]->classifyCollaborator(houseMaster1, houseMaster1.getClients()["client2"]->getAssociatedInterventions(houseMaster1).at(0)->getCollabId(), savior);
}