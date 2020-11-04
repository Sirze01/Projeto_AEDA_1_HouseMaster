#ifndef SRC_DATE_H
#define SRC_HOUSEMASTER_DATE_H

#include <string>
#include <stdexcept>
#include <sstream>


struct date {
    unsigned int day{};
    unsigned int month{};
    unsigned int year{};
    unsigned int hours{};
    unsigned int minutes{};

    date();

    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes,
         bool valid= true);

    date(const std::string &date);

    void readDuration(const std::string &duration);

    int getDaysInMonth() const;

    bool isValidDate(bool throwExcept=false);

    class InvalidDate;

    std::string dateToStr() const;

    date operator+(const date &d1) const;

    bool operator==(const date &d2) const;

    bool operator<(const date &d2) const;

    bool operator>(const date &d2) const;
};


class date::InvalidDate : public std::invalid_argument {
public:
    explicit InvalidDate(const std::string &error_msg);
};

#endif
