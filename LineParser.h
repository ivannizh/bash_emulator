#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <iostream>
#include <map>
#include <regex>
#include <algorithm>

#include "myvector.h"

class LineParser {
public:
    LineParser() : comand_("") { }

    typedef MyVector<std::pair<std::string, std::string>> Param;

    void parse(const std::string &str);

    const std::string getComand() const;
    const MyVector<std::__cxx11::string> &getArgs() const;

    int getParamsSize() const;
    int getArgsSize();

    std::string getParam(const std::string &p);

    const Param& getParams() const;

private:
    std::string comand_;
    Param params_;
    MyVector<std::string>   args_;

    void parseLine(std::string line);
    std::string trimLine(const std::string &str);
};

#endif // LINEPARSER_H
