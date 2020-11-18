#ifndef SRC_DATE_H
#define SRC_HOUSEMASTER_DATE_H

#include <string>
#include <stdexcept>
#include <sstream>
#include <iomanip>

/** \struct date
 *  \brief A struct to assign dates and time.
 *  This struct sets the dates used during the interventions*/
struct date {
    unsigned int day{}; /**<day */
    unsigned int month{}; /**<month*/
    unsigned int year{};/**<year*/
    unsigned int hours{};/**<hours*/
    unsigned int minutes{};/**<minutes*/

    /**date's empty constructor*/
    date();
    /**date's constructor using numbers*/
    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes);
    /**date's constructor using a string*/
    date(const std::string &date);
    /**A normal member taking no arguments and returning an unsigned integer value
     * \return The number of days that a specific month has*/
    unsigned int getDaysInMonth() const;
    /** A normal member taking no arguments and returning a string value
     * \return The string value of a date*/
    std::string dateToStr() const;
    /** An operator that adds two dates and returns the resulting date
     * \return The added date*/
    date operator+(const date &d1) const;
    /** An operator that compares two dates and returns whether they are equal
     * \return The compared dates are equal or not*/
    bool operator==(const date &d2) const;
    /** An operator that returns whether the first date is before the second one
     * \return The first date is before the second or the contrary*/
    bool operator<(const date &d2) const;
    /** An operator that returns whether the first date is after the second one
     * \return The first date is after the second or the contrary*/
    bool operator>(const date &d2) const;
    /** A normal member that checks whether the date is valid or not
     * \param throwExcept is used only when we want to throw an exception. On the
     * normal code flow it is unnecessary since throwing the exception will stop
     * the program, so by default we use it as false
     * \return Whether the duration is valid or invalid*/
    bool isValidDate(bool throwExcept=false) const;
    /**\class invalidDate
     * \brief Class used to throw an exception warning about Invalid Dates*/
    class InvalidDate;
};


/**\class InvalidDate
 * \brief A class to set invalid dates.
 * This class is used to throw exceptions related to invalid dates*/
class date::InvalidDate : public std::invalid_argument {
public:
    explicit InvalidDate(const std::string &error_msg);
};

/**\struct duration
 * \brief A struct to assign durations.
 * This struct sets the interventions' durations*/
struct duration: public date
{
    /**duration's empty constructor*/
    duration();
    /**duration constructor using numbers*/
    duration(unsigned int hours, unsigned int minutes);
    /**duration constructor using a string*/
    duration(const std::string &duration);
    /** A normal member taking no arguments and returning a string value
    * \return The string value of a duration*/
    std::string durationToStr() const;
    /** A normal member that checks whether the duration is valid or not
     * \param throwExcept is used only when we want to throw an exception. On the
     * normal code flow it is unnecessary since throwing the exception will stop
     * the program, so by default we use it as false
     * \return Whether the duration is valid or invalid*/
    bool isValidDuration(bool throwExcept=false) const;
    /**\class invalidDate
     * \brief Class used to throw an exception warning about Invalid Dates*/
    class InvalidDuration;
};


/**\class InvalidDuration
 * \brief A class to set invalid durations.
 * This class is used to throw exceptions related to invalid durations*/
class duration::InvalidDuration : public std::invalid_argument
{
public:
    explicit InvalidDuration(const std::string &error_msg);
};


#endif
