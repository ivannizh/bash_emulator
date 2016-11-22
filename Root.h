#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "User.h"
//#include "Files/File.h"
#include "Files/Catalog.h"
#include "LineParser.h"
//#include "CommandArray.h"

class Root {
public: 
    Root() : curDir_(rootDir_){

        comands_["showusers"] = &Root::showUsers;
        comands_["adduser"]   = &Root::addUser;
        comands_["deluser"]   = &Root::deleteUser;
        comands_["touch"]     = &Root::mkFile;
        comands_["mkdir"]     = &Root::mkdir;
        comands_["exit"]      = &Root::exit;
        comands_["cp"]        = &Root::cp;
        comands_["rm"]        = &Root::rm;
        comands_["cd"]        = &Root::cd;
        comands_["mv"]        = &Root::mv;
        comands_["ls"]        = &Root::ls;
        comands_[">"]         = &Root::mkFile;
    }

    typedef void(Root::*funcPtr)();

    void startWork(std::istream &is) {
        std::string line;
        while(true){
            line = "";
            std::getline(is, line);
            if (line.length() == 0)
                continue;
            lParser.parse(line);
            if(comands_.count(lParser.getComand()))
                (this->*comands_[lParser.getComand()])();
            else
                std::cout << "No command '" << lParser.getComand() << "' found" << std::endl;
        }
    }
private: 
    User curUser_;
    Catalog rootDir_; // не удалять
    Catalog& curDir_; // не удалять
    std::vector<User> users_;
    std::vector<UserGroup> groups_;
    std::map<std::string,funcPtr> comands_;
    enum descrType {FILE, CATALOG};

    LineParser lParser;

    void mkFile () { newDescriptor(descrType::FILE);    }
    void mkdir  () { newDescriptor(descrType::CATALOG); }

    void newDescriptor(descrType t){
        if (lParser.getParams().size() > 0){
            std::cout << "Ignoring parametres" << std::endl;
        }
        for(const auto name: lParser.getArgs())
            switch (t) {
            case descrType::CATALOG:
                curDir_.creatCatalog(name);
                break;
            case descrType::FILE:
                curDir_.creatFile(name);
                break;
            default:
                std::cerr << "ERROR in func Root::newDescriptor" << std::endl;
                break;
            }
    }

    void ls         ( ) {
        std::clog << "in ls func" << std::endl;
        curDir_.showCatalog();
    }

    void addUser    ( ) { std::clog << "in addUser func" << std::endl; }
    void deleteUser ( ) { std::clog << "in deleteUser func" << std::endl; }
    void cp         ( ) { std::clog << "in cp func" << std::endl; }
    void rm         ( ) { std::clog << "in rm func" << std::endl; }
    void exit       ( ) { std::clog << "in exit func" << std::endl; }
    void showUsers  ( ) { std::clog << "in showUsers func" << std::endl; }
    void cd         ( ) { std::clog << "in cd func" << std::endl; }
    void mv         ( ) { std::clog << "in mv func" << std::endl; }


    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
