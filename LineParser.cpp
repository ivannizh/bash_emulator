#include "LineParser.h"

std::string trimLine(const std::string &str){
    std::string tmp = str;
    while (tmp[0] == ' ')
        tmp.erase(0, 1);
    while (tmp[tmp.length()-1] == ' ')
        tmp.erase((tmp.length()-1, 1));
    return tmp;
}

void LineParser::parseLine() {

    std::string line = line_;

    const std::regex reCommand("^[^ ]+");
    const std::regex reParams(" (--|-)[\\w\\.\\/]+ [\\w\\.\\/]+| (--|-)[\\w\\.\\/-]+");
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
        if (std::regex_search(tmp, mLoc, std::regex("^[-\\w]+ [\\w\\/\\.]+"))){
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

        qDebug() << "command :\n     " << QString::fromStdString(comand_);

        qDebug() << "params :";
        for(auto param: params_)
            qDebug() << "     " << QString::fromStdString(param.first) << QString::fromStdString(param.second);

        qDebug() << "args :";
        for(auto arg: args_)
            qDebug() << "     " << QString::fromStdString(arg);

        qDebug() << "line : " << QString::fromStdString(line);

}

std::string LineParser::getComand() const
{
    return comand_;
}
