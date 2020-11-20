#include <gtest/gtest.h>
#include "../src/Services.h"

using testing::Eq;

TEST(Date, leap_year){
    EXPECT_EQ(Date(1, 2, 2020, 20, 00).getDaysInMonth(), 29);

    EXPECT_EQ(Date(1, 2, 2021, 20, 00).getDaysInMonth(), 28);

}

TEST(Date, month_days){
    EXPECT_EQ(Date(1, 1, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 3, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 4, 2020, 00, 00).getDaysInMonth(), 30);


    EXPECT_EQ(Date(1, 5, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 6, 2020, 00, 00).getDaysInMonth(), 30);


    EXPECT_EQ(Date(1, 7, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 8, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 9, 2020, 00, 00).getDaysInMonth(), 30);

    EXPECT_EQ(Date(1, 10, 2020, 00, 00).getDaysInMonth(), 31);


    EXPECT_EQ(Date(1, 11, 2020, 00, 00).getDaysInMonth(), 30);


    EXPECT_EQ(Date(1, 12, 2020, 00, 00).getDaysInMonth(), 31);

}
/*
TEST(Date, valid_date){
    date val2;
    val2.day = 25;
    val2.month = 06;
    val2.year = 2020;
    val2.hours = 9;
    val2.minutes = 0;
    EXPECT_EQ(date(25,06,2020, 9,00), val2);

    date val1(25,06,2020,9,0);
    EXPECT_EQ(val1, val2);

    EXPECT_THROW(date d1(31, 2, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(31, 4, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(30, 13, 2020, 20,0), date::InvalidDate);

    EXPECT_THROW(date d1(32, 1, 2020, 24,0), date::InvalidDate);

    EXPECT_THROW(date d1(32, 1, 2020, 20,60), date::InvalidDate);
}*/

TEST(DATE, print_date){
    EXPECT_EQ(Date(12, 12, 2012, 12, 51).getString(), "12/12/2012  12:51");
}

TEST(Date, date_operators){
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) == Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) == Date(1, 1, 2020, 00, 05), false);

    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 01) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 01, 00) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(2, 1, 2020, 00, 00) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 2, 2020, 00, 00) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2021, 00, 00) < Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(1, 1, 2020, 00, 01), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(1, 1, 2020, 01, 00), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(2, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(1, 2, 2020, 00, 01), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) < Date(1, 1, 2021, 00, 01), true);

    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(1, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 01) > Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 1, 2020, 01, 00) > Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(2, 1, 2020, 00, 00) > Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 2, 2020, 00, 00) > Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 1, 2021, 00, 00) > Date(1, 1, 2020, 00, 00), true);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(1, 1, 2020, 00, 01), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(1, 1, 2020, 01, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(2, 1, 2020, 00, 00), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(1, 2, 2020, 00, 01), false);
    EXPECT_EQ(Date(1, 1, 2020, 00, 00) > Date(1, 1, 2021, 00, 01), false);


    //add dates
    EXPECT_EQ(Date(1, 10, 2020, 20, 30) + Date(1, 1, 1, 0, 0), Date(2, 11, 2021, 20, 30));
    EXPECT_EQ(Date(31, 10, 2020, 23, 50) + Date(1, 1, 0, 0, 0), Date(1, 11, 2020, 23, 50));
    EXPECT_EQ(Date(31, 12, 2020, 23, 50) + Date(1, 1, 0, 0, 0), Date(1, 1, 2021, 23, 50));
    EXPECT_EQ(Date(29, 2, 2020, 23, 50) + Date(1, 1, 0, 0, 0), Date(1, 3, 2020, 23, 50));
    EXPECT_EQ(Date(28, 2, 2019, 23, 50) + Date(1, 1, 0, 0, 0), Date(1, 3, 2019, 23, 50));


//add dates to duration
    EXPECT_EQ(Date(1, 10, 2020, 20, 30) + Duration(0, 15), Date(1, 10, 2020, 20, 45));  // add 15 min to date
    EXPECT_EQ(Date(1, 10, 2020, 20, 50) + Duration(0, 15), Date(1, 10, 2020, 21, 05));  // add 15 min to end of hours
    EXPECT_EQ(Date(1, 10, 2020, 23, 50) + Duration(0, 15), Date(2, 10, 2020, 0, 5));    // add 15 min to end of the day
    EXPECT_EQ(Date(31, 10, 2020, 23, 50) + Duration(0, 15), Date(1, 11, 2020, 0, 5));   // add 15 min to end of month
    EXPECT_EQ(Date(31, 12, 2020, 23, 50) + Duration(0, 15), Date(1, 1, 2021, 0, 5));    // add 15 min to end of year
    EXPECT_EQ(Date(29, 2, 2020, 23, 50) + Duration(0, 15), Date(1, 3, 2020, 0, 5));     // add 15 min to end of february

}