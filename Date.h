#ifndef _DATE_H
#define _DATE_H

#include <ctime>
#include <iostream>
#include <iomanip>

class Date {
public:
    Date() {
        time_t t = time(0);
        struct tm * now = localtime( & t );
        year_ = now->tm_year + 1900;
        month_ = now->tm_mon;
        day_ = now->tm_mday;
        hour_ = now->tm_hour;
        min_ = now->tm_min;
        sec_ = now->tm_sec;
    }
    
    void setCurTime(){
        time_t t = time(0);
        struct tm * now = localtime( & t );
        year_ = now->tm_year + 1900;
        month_ = now->tm_mon;
        day_ = now->tm_mday;
        hour_ = now->tm_hour;
        min_ = now->tm_min;
        sec_ = now->tm_sec;
    }

    friend std::ostream& operator<<(std::ostream& os, const Date& d) {
        os << d.months[d.month_] << " " << std::setw(2) << d.day_ << " ";
        os.fill('0');
        os << std::setw(2) << d.hour_ << ":" << std::setw(2) << d.min_;
        return os;
    }

private: 
    int day_;
    int month_;
    int year_;
    int hour_;
    int min_;
    int sec_;

    const std::string months[12] = { "янв", "фев", "мар", "апр", "май", "июн", "июл", "авг", "сен", "окт", "ноя", "дек" };
};

#endif //_DATE_H
