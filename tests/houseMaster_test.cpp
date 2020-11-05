#include <gtest/gtest.h>
#include "../src/HouseMaster.h"

using testing::Eq;

/*TEST(HouseMaster, defaultConstructor){

}*/

TEST(HouseMaster, fileConstructor){
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    std::vector<Service> svc;
    svc.push_back(Service("ajudar os amiguinhos com fisica", false, 3, date(0,0,0,2,30,0)));
    svc.push_back(Service("ajudar os amiguinhos com amat", false, 3, date(0,0,0,3,0,0)));
    svc.push_back(Service("jantar na cantina as 18h30", false, 2.70, date(0,0,0,1,0,0)));
    svc.push_back(Service("montar um carro do nada", false, 2.5, date(0,0,0,10,0,0)));
    svc.push_back(Service("desmontar um computador", false, 4, date(0,0,0,2,0,0)));
    svc.push_back(Service("canalizador", true, 5, date(0,0,0,3,0,0)));
    svc.push_back(Service("andar a correr atras de uma princesa ou la o que e", false, 1, date(0,0,0,4,0,0)));
    for(size_t i = 0; i < svc.size(); i++){
        EXPECT_EQ(svc.at(i).name, houseMaster1.getAvailableServices().at(i)->name);
        EXPECT_EQ(svc.at(i).basePrice, houseMaster1.getAvailableServices().at(i)->basePrice);
        EXPECT_EQ(svc.at(i).duration, houseMaster1.getAvailableServices().at(i)->duration);
        EXPECT_EQ(svc.at(i).pro, houseMaster1.getAvailableServices().at(i)->pro);
    }



    std::vector<Client> cltVec;
    cltVec.push_back(Client(1111232, "cliente muito simpatico", false));
    cltVec.push_back(Client(374435, "cliente que faz comentarios racistas", false));
    cltVec.push_back(Client(999999999, "cliente que nao se lembra do nome", true));
    cltVec.push_back(Client(5045085040508, "adriano malheiro caloteiro", false));

    for(size_t i = 0; i<cltVec.size(); i++){
        EXPECT_EQ(cltVec.at(i).getName(), houseMaster1.getClients().at(i)->getName());
        EXPECT_EQ(cltVec.at(i).getNif(), houseMaster1.getClients().at(i)->getNif());
        EXPECT_EQ(cltVec.at(i).getAssociatedInterventions(), houseMaster1.getClients().at(i)->getAssociatedInterventions());
        EXPECT_EQ(cltVec.at(i).getRequestedInterventions(), houseMaster1.getClients().at(i)->getRequestedInterventions());
    }

    std::vector<Collaborator> colVec;
    std::vector<Service*> tmpvec;
    tmpvec.push_back(houseMaster1.findServiceByName("ajudar os amiguinhos com fisica"));
    tmpvec.push_back(houseMaster1.findServiceByName("ajudar os amiguinhos com amat"));
    tmpvec.push_back(houseMaster1.findServiceByName("jantar na cantina as 18h30"));
    colVec.push_back(Collaborator(tmpvec, "Miro Osvaldo", true));
    tmpvec.clear();
    tmpvec.push_back(houseMaster1.findServiceByName("montar um carro do nada"));
    tmpvec.push_back(houseMaster1.findServiceByName("desmontar um computador"));
    colVec.push_back(Collaborator(tmpvec, "Jose Retiros", false));
    tmpvec.clear();
    tmpvec.push_back(houseMaster1.findServiceByName("canalizador"));
    tmpvec.push_back(houseMaster1.findServiceByName("andar a correr atras de uma princesa ou la o que e"));
    colVec.push_back(Collaborator(tmpvec, "Super Mario", true));
    for(size_t i = 0; i < colVec.size(); i++){
        EXPECT_EQ(colVec.at(i).getName(), houseMaster1.getCollaborators().at(i)->getName());
        EXPECT_EQ(colVec.at(i).getScore(), houseMaster1.getCollaborators().at(i)->getScore());
        EXPECT_EQ(colVec.at(i).getServices(), houseMaster1.getCollaborators().at(i)->getServices());
        EXPECT_EQ(colVec.at(i).isPro(), houseMaster1.getCollaborators().at(i)->isPro());
    }
    
}

/*
TEST(HouseMaster, getters){
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));


}

TEST(HouseMaster, availableServicesManip){

}

TEST(HouseMaster, updateInterventions){

}

TEST(HouseMaster, sortCollaboratorsByScore){

}

TEST(HouseMaster, assignCollaborator){
    HouseMaster houseMaster1(std::ifstream("../../data/collabs.txt"),
                             std::ifstream("../../data/clients.txt"),
                             std::ifstream("../../data/services.txt"));

    houseMaster1.getClients().at(0)->requestIntervention(date(23,12,2020,9,30),
                                                         *houseMaster1.getAvailableServices().at(0));
}

TEST(HouseMaster, UnnavailableAppointment){

}

TEST(HouseMaster, InexistentService){

}