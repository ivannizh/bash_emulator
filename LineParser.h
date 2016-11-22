#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>
#include <QDebug>

class LineParser
{
public:
    LineParser() : line_(""), comand_("") { }

    void parse(const std::string &str){
        line_ = str;
        parseLine();
    }

    std::string getComand() const;

private:
    void parseLine();

    std::string line_;
    std::string comand_;
    std::vector<std::pair<std::string, std::string>> params_;
    std::vector<std::string> args_;
};

#endif // LINEPARSER_H
