#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>

class LineParser
{
public:
    LineParser() : comand_("") { }

    void parse(const std::string &str){
//        line_ = str;
        comand_ = "";
        params_.resize(0);
        args_.resize(0);
        parseLine(str);
    }

    const std::string getComand() const;
    const std::vector<std::string> getArgs() const;
    int getParamsSize() const {
        return params_.size();
    }

    int getArgsSize(){
        return args_.size();
    }


    std::string getParam(const std::string &p){

        for (auto i = params_.begin(); i != params_.end(); i++)
            if (i->first == p){
                std::string tmp = i->second;
                params_.erase(i);
                i--;

                if (tmp == "")
                    tmp = "\n";

                return tmp;
            }
        return "";
    }

private:
    void parseLine(std::string line);

    //    std::string line_;
    std::string comand_;
    std::vector<std::pair<std::string, std::string>> params_;
    std::vector<std::string> args_;
};

#endif // LINEPARSER_H
