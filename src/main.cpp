#include <iostream>
#include "HouseMaster.h"
#include "Individual.h"


int main() {



    HouseMaster houseMaster(std::ifstream("data/collabs.txt"), std::ifstream("data/clients.txt"));

    Collaborator* w1 = houseMaster.getCollaborators().front();
    Collaborator* w2 = houseMaster.getCollaborators().back();

   std::cout << "Worker 1\nName : " << w1->getName() << "\nStarting Score : " << w1->getScore() << "\n\n";
   std::cout << "Worker 2\nName : " << w2->getName() << "\nStarting Score : " << w2->getScore() << "\n\n";

   w1->addClassification(clumsy);
   w1->addClassification(savior);
   w2->addClassification(attentive);
   w2->addClassification(unreliable);

   w1->updateScore();
   w2->updateScore();

    std::cout << "Worker 1\nName : " << w1->getName() << "\nEnding Score : " << w1->getScore() << "\n\n";
    std::cout << "Worker 2\nName : " << w2->getName() << "\nEnding Score : " << w2->getScore() << "\n\n";


/*
    // TESTING DATE
    std::cout << "____DATE____" << std::endl;
*/


    /*
    // Leap year
    date first_date;
    first_date.day = 2;
    first_date.month = 2;
    first_date.year = 2024;
    first_date.hours = 20;
    first_date.minutes = 30;
    std::cout << "Days in February 2024: " << first_date.getDaysInMonth() << std::endl;

    //Non leap year
    date second_date;
    second_date.day = 1;
    second_date.month = 2;
    second_date.year = 2025;
    second_date.hours = 00;
    second_date.minutes = 15;
    std::cout << "Days in February 2025: " << second_date.getDaysInMonth() << std::endl;

    */

/*
    // add 15 min to date
    date first_date(1,12,2020,23,59);

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes<< "min" << std::endl;

    date second_date(0,0,0,0,15, 0);

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/


/*
    // add 15 min to end of hours
    date first_date;
    first_date.day = 1;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hours = 20;
    first_date.minutes = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes << "min" << std::endl;

    date second_date;
    second_date.day = 1;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hours = 00;
    second_date.minutes = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/


/*
    // add 15 min to end of the day
    date first_date;
    first_date.day = 1;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hours = 23;
    first_date.minutes = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes << "min" << std::endl;

    date second_date;
    second_date.day = 1;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hours = 00;
    second_date.minutes = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/


/*
    // add 15 min to end of the day and month
    date first_date;
    first_date.day = 31;
    first_date.month = 10;
    first_date.year = 2020;
    first_date.hours = 23;
    first_date.minutes = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes << "min" << std::endl;

    date second_date;
    second_date.day = 31;
    second_date.month = 10;
    second_date.year = 2020;
    second_date.hours = 00;
    second_date.minutes = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/


/*
// add 15 min to end of the day, month and year
    date first_date;
    first_date.day = 31;
    first_date.month = 12;
    first_date.year = 2020;
    first_date.hours = 23;
    first_date.minutes = 50;

    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes << "min" << std::endl;

    date second_date;
    second_date.day = 31;
    second_date.month = 12;
    second_date.year = 2020;
    second_date.hours = 00;
    second_date.minutes = 15;

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/


/*
// add 15 min to end of the day and month in february
    date first_date(28,2,2019,23,1);


    std::cout << "Date 1: " << first_date.day << "-" <<  first_date.month << "-" << first_date.year << "  -  " <<
        first_date.hours << "h" << first_date.minutes << "min" << std::endl;

    date second_date(27,2,2019,00,15);

    std::cout << "Date 2: " << second_date.day << "-" <<  second_date.month << "-" << second_date.year << "  -  " <<
        second_date.hours << "h" << second_date.minutes << "min" << std::endl;


    date final_date = first_date + second_date;
    std::cout << "Final date: " << final_date.day << "-" <<  final_date.month << "-" << final_date.year << "  -  " <<
        final_date.hours << "h" << final_date.minutes << "min" << std::endl;
*/

    return 0;
}
