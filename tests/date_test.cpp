#include <gtest/gtest.h>
#include "../src/Services.h"

using testing::Eq;

TEST(Date, valid_date){
    EXPECT_THROW(date d1(29,19,2019,23,53),date::InvalidDate);
    date d1(29,02,2020,23,53);
}
