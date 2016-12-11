#ifndef _DATE_H
#define _DATE_H

#include <ctime>
#include <iostream>
#include <iomanip>

class Date {
public:
    Date();
    
    void setCurTime();

    friend std::ostream& operator<<(std::ostream& os, const Date& d);

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
