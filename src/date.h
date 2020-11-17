#ifndef SRC_DATE_H
#define SRC_HOUSEMASTER_DATE_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>


struct date {
    unsigned int day{};
    unsigned int month{};
    unsigned int year{};
    unsigned int hours{};
    unsigned int minutes{};

    date();

    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes);

    date(const std::string &date);

    unsigned int getDaysInMonth() const;

    bool isValidDate(bool throwExcept=false) const;

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


//duration

struct duration: public date
{
    duration();

    duration(unsigned int hours, unsigned int minutes);

    duration(const std::string &duration);

    bool isValidDuration() const;

    class InvalidDuration;

    std::string durationToStr() const;
};


class duration::InvalidDuration : public std::invalid_argument
{
public:
    explicit InvalidDuration(const std::string &error_msg);
};


#endif
