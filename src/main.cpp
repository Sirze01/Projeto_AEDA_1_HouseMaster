#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"
#include "Services.h"

int main() {
    /*HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    Client* c1 = houseMaster.getClients().front();
    Client* c2 = houseMaster.getClients().back();
    date d1{};
    d1.day = 24; d1.hours = 1; d1.minutes = 3; d1.month = 10; d1.year = 2020;

    Service* service1 = houseMaster.getCollaborators().front()->getServices().front();
    Service* service2 = houseMaster.getCollaborators().back()->getServices().back();

    c1->requestIntervention(d1, *service1, false);
    c1->requestIntervention(d1, *service2, false);
    c2->requestIntervention(d1, *service2, false);
    houseMaster.updateInterventions();

    for(const auto & service: houseMaster.getAvailableServices()){
        std::cout << "\t" << service->name << "\n";
    }

    std::cout << "REGISTERED INTERVENTIONS:\n";



    for (const auto &i : houseMaster.getInterventions()) {
        std::cout << i.first->getName() << " requested " << i.second->getService().name << "\n";
        for (const auto &collab : houseMaster.getCollaborators()) {
            auto *service = new Service(i.second->getService());
            std::cout << collab->getName() << " Can Preform ? " << collab->canPreform(service) << "\n";
        }
        std::cout << "\n";
    }*/



    // TESTING DATE
    std::cout << "____DATE____" << std::endl;


    // Leap year
    date first_date;
    first_date.day = 2;
    first_date.month = 2;
    first_date.year = 2020;
    first_date.hours = 20;
    first_date.minutes = 30;
    std::cout << "Days in February 2020: " << first_date.getDaysInMonth() << std::endl;

    //Non leap year
    date second_date;
    second_date.day = 1;
    second_date.month = 2;
    second_date.year = 2018;
    second_date.hours = 00;
    second_date.minutes = 15;
    std::cout << "Days in February 2018: " << second_date.getDaysInMonth() << std::endl;



    /*// add 15 min to date
    date first_date;
    first_date.day = 1;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hour = 20;
    first_date.minute = 30;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 1;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;

*/
/*
    // add 15 min to end of hours
    date first_date;
    first_date.day = 1;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hour = 20;
    first_date.minute = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 1;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;


*/
/*

    // add 15 min to end of the day
    date first_date;
    first_date.day = 1;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hour = 23;
    first_date.minute = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 1;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;
*/
/*
    // add 15 min to end of the day and month
    date first_date;
    first_date.day = 31;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hour = 23;
    first_date.minute = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 31;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;
*/
/*

// add 15 min to end of the day, month and year
    date first_date;
    first_date.day = 31;
    first_date.month = 12;
    first_date.year = 2020;
    first_date.hour = 23;
    first_date.minute = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 31;
    second_date.month = 12;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;
*/
/*

// add 15 min to end of the day and month in february
    date first_date;
    first_date.day = 29;
    first_date.month = 2;
    first_date.year = 2020;
    first_date.hour = 23;
    first_date.minute = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " << first_date.hour << "h" << first_date.minute << "min" << std::endl;

    date second_date;
    second_date.day = 29;
    second_date.month = 2;
    second_date.year = 2020;
    second_date.hour = 00;
    second_date.minute = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " << second_date.hour << "h" << second_date.minute << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " << final_date.hour << "h" << final_date.minute << "min" << std::endl;
*/



    return 0;
}
