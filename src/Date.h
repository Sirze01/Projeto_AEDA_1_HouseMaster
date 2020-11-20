#ifndef SRC_DATE_H
#define SRC_DATE_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

/**
 * @brief A class to represent dates.
 */
class Date {


public:
    Date();

    Date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes);

    explicit Date(const std::string &date);

    virtual std::string getString() const;

    Date operator+(const Date &d1) const;

    bool operator==(const Date &d2) const;

    bool operator<(const Date &d2) const;

    bool operator>(const Date &d2) const;

    virtual void checkIfValid() const;

    class InvalidDate;

protected:
    unsigned int _hours{};
    unsigned int _minutes{};
private:
    bool isLeapYear() const;

    unsigned int getDaysInMonth() const;

    unsigned int _day{};
    unsigned int _month{};
    unsigned int _year{};
};


/**\class InvalidDate
 * \brief A class to set invalid dates.
 * This class is used to throw exceptions related to invalid dates*/
class Date::InvalidDate : public std::invalid_argument {
public:
    explicit InvalidDate(const std::string &error_msg);
};


/**
 * @brief A class to represent durations, inherits from Date.
 */
class Duration : public Date {
public:
    Duration();

    Duration(unsigned int hours, unsigned int minutes);

    explicit Duration(const std::string &duration);

    std::string getString() const override;

    void checkIfValid() const override;

    class InvalidDuration;

private:

};


/**
 * @brief This class is used to throw exceptions related to invalid durations
 */
class Duration::InvalidDuration : public std::invalid_argument {
public:
    explicit InvalidDuration(const std::string &error_msg);
};


#endif
