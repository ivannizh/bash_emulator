#include "LineParser.h"

std::string LineParser::trimLine(const std::string &str) {
    std::string tmp = str;
    while (tmp[0] == ' ')
        tmp.erase(0, 1);
    while (tmp[tmp.length()-1] == ' ')
        tmp.erase(tmp.length()-1, 1);
    return tmp;
}

void LineParser::parseLine(std::string line) {

    const std::regex reCommand("^[^ ]+");
    const std::regex reParams(" (--|-)[\\w\\.\\/+-]+ [\\w\\.\\/]+| (--|-)[\\w\\.\\/+-]+");
    const std::regex reArgs(" [\\w\\.\\/:*-]+");

    std::smatch match;

    if (std::regex_search(line, match, reCommand)) {
        comand_ = trimLine(match[0]);
        line.erase(line.find(comand_), comand_.length());
    }
    match = std::smatch();
    while (std::regex_search(line, match, reParams)) {
        std::string tmp = trimLine(match[0]);
        std::smatch mLoc;
        if (std::regex_search(tmp, mLoc, std::regex("^[-\\w+-]+ [\\w\\/\\.]+"))){
            int index = tmp.find(' ');
            params_.push_back(std::pair<std::string, std::string>(tmp.substr(1, index-1), tmp.substr(index+1, tmp.length()-1)));
        } else {
            params_.push_back(std::pair<std::string, std::string>(tmp.substr(1, tmp.length()-1), ""));
        }
        line.erase(line.find(match[0]), match[0].length());
    }
    while (std::regex_search(line, match, reArgs)) {
        args_.push_back(trimLine(match[0]));
        line.erase(line.find(match[0]), match[0].length());
    }


//    std::clog <<"\n\n\n\nNew command:\n\n";
//    std::clog << "command :\n     " << comand_ << std::endl;

//    std::clog << "params :\n";
//    for(auto param: params_)
//        std::clog << "     <" << param.first << "> <" << param.second << ">" << std::endl;

//    std::clog << "args :\n";
//    for(auto arg: args_)
//        std::clog << "     " << arg << std::endl;

//    std::clog << "line : " << line << std::endl;

}

const MyVector<std::string>& LineParser::getArgs() const {
    return args_;
}

int LineParser::getParamsSize() const {
    return params_.size();
}

int LineParser::getArgsSize(){
    return args_.size();
}

std::__cxx11::string LineParser::getParam(const std::__cxx11::string &p){

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

const MyVector<std::pair<std::string, std::string>>& LineParser::getParams() const {
    return params_;
}

void LineParser::parse(const std::__cxx11::string &str){
    comand_ = "";
    params_.clear();
    args_.clear();
    parseLine(str);
}

const std::string LineParser::getComand() const {
    return comand_;
}
