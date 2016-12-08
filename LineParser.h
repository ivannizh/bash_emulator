#ifndef LINEPARSER_H
#define LINEPARSER_H

#include <iostream>
#include <vector>
#include <map>
#include <regex>
#include <algorithm>

class LineParser {
public:
    LineParser() : comand_("") { }

    typedef std::vector<std::pair<std::string, std::string>> Param;

    void parse(const std::string &str){
        comand_ = "";
        params_.resize(0);
        args_.resize(0);
        parseLine(str);
    }

    const std::string getComand() const;
    const std::vector<std::__cxx11::string> &getArgs() const;

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

    const Param& getParams() const;

private:
    std::string comand_;
    Param params_;
    std::vector<std::string> args_;

    void parseLine(std::string line);
    std::string trimLine(const std::string &str);
};

#endif // LINEPARSER_H
