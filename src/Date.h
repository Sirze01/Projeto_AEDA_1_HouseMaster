#ifndef SRC_DATE_H
#define SRC_DATE_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <vector>

/**
 * @brief A class to represent dates.
 */
class Date {
public:
    Date();

    Date(int day, int month, int year, int hours, int minutes);

    explicit Date(const std::string &date);

    tm getDate() const;

    virtual std::string getString() const;

    std::vector<std::string> getWeekdays();

    Date operator+(const Date &d1) const;

    Date operator-(const Date &d1) const;

    bool operator==(const Date &d1) const;

    bool operator<(const Date &d1) const;

    bool operator>(const Date &d1) const;

    class InvalidDate;



protected:
    tm _date{};
    std::vector<std::string> _weekdays {"sunday", "monday", "tuesday", "wednesday", "thursday", "friday", "saturday"};


private:

    static void checkIfValid(int &day, int &month, int &year, int &hours, int &minutes) ;
    static bool isLeapYear(int &year) ;
    static int getDaysInMonth(int day, int month, int year, int hours, int minutes);
    static std::string getString_err( int &day,  int &month,  int &year,  int &hours,  int &minutes) ;

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

    Duration( int hours,  int minutes);

    explicit Duration(const std::string &duration);

    tm getDuration() const;

    std::string getString() const override;

};

class Availability : public Date{
public:
    Availability();

    Availability(int weekday);

    explicit Availability(const std::string &weekday);

    int getWeekday() const;

    std::string getString() const override;

    bool operator<(const Availability &d1) const;
private:
    static void checkIfValid(int day);
};
/**
 * @brief This class is used to throw exceptions related to invalid durations
 */
 /*
class Duration::InvalidDuration : public std::invalid_argument {
public:
    explicit InvalidDuration(const std::string &error_msg);
};
  */


#endif
