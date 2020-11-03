#ifndef SRC_SERVICES_H
#define SRC_SERVICES_H

#include<stdexcept>
#include<string>
#include<sstream>
#include<iostream>

struct date {
    unsigned int day{};
    unsigned int month{};
    unsigned int year{};
    unsigned int hours{};
    unsigned int minutes{};

    date();

    date(unsigned int day, unsigned int month, unsigned int year, unsigned int hours, unsigned int minutes,
          bool valid= true);

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

struct Service {
    std::string name;
    bool pro;
    float basePrice;
    date duration;
    Service(const std::string &name, bool pro, float basePrice, const date &duration);
};


enum processState {
    WaitingPayment,
    Scheduled,
    InProgress,
    Concluded
};

class Intervention {
private:
    unsigned int _id;
    date _startingTime;
    Service _type;
    bool _forcePro;
    processState _state;
    float _price;
    unsigned int _collabId;

public:
    static unsigned int _idSeq;

    Intervention(date appointment, Service type, bool forcePro);

    const date *getStartingTime() const;

    Service* getService();

    bool getForcePro() const;

    unsigned int getCollabId() const;

    void setCollabId(unsigned int collabId);

    processState getProcessState();

    bool conflictsWith(date start, date duration);

    float getPrice() const;

    date getEndTime() const;
};

#endif
