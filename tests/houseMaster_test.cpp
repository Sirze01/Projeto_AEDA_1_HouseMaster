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
    svc.emplace_back("ajudar os amiguinhos com fisica", false, 3, date(0,0,0,2,30,false));
    svc.emplace_back("ajudar os amiguinhos com amat", false, 3, date(0,0,0,3,0,false));
    svc.emplace_back("jantar na cantina as 18h30", false, 2.70, date(0,0,0,1,0,false));
    svc.emplace_back("montar um carro do nada", false, 2.5, date(0,0,0,10,0,false));
    svc.emplace_back("desmontar um computador", false, 4, date(0,0,0,2,0,false));
    svc.emplace_back("canalizador", true, 5, date(0,0,0,3,0,false));
    svc.emplace_back("andar a correr atras de uma princesa ou la o que e", false, 1, date(0,0,0,4,0,false));
    for(auto & i : svc){
        EXPECT_EQ(i.name, houseMaster1.getAvailableServices()[i.name]->name);
        EXPECT_EQ(i.pro, houseMaster1.getAvailableServices()[i.name]->pro);
        EXPECT_EQ(i.duration, houseMaster1.getAvailableServices()[i.name]->duration);
        EXPECT_EQ(i.basePrice, houseMaster1.getAvailableServices()[i.name]->basePrice);
    }


    Client::_idSeqClt= 0 ;
    std::vector<Client> cltVec;
    cltVec.emplace_back(1111232, "mafarrico", false);
    cltVec.emplace_back(374435, "cliente que faz comentarios racistas", false);
    cltVec.emplace_back(999999999, "cliente que nao se lembra do nome", true);
    cltVec.emplace_back(5045085040508, "adriano malheiro caloteiro", false);

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
    houseMaster1.addAvailableService("ajudar os amiguinhos com fisica", false, 3, date(0,0,0,2,30,false));
    EXPECT_FALSE(houseMaster1.getAvailableServices().find("ajudar os amiguinhos com fisica") == houseMaster1.getAvailableServices().end());

    // Try to add existent service
    EXPECT_THROW(houseMaster1.addAvailableService("ajudar os amiguinhos com fisica", false, 3, date(0,0,0,2,30,false)),HouseMaster::ExistentService);
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
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    // add Intervention
}


TEST(HouseMasterTest, usageTest){
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    // remove client
    // add client
}