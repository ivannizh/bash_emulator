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

    typedef void(Root::*funcPtr)(void);

    void startWork(std::istream &is) {
        std::string line;
        while(true){
            line = "";
            std::getline(is, line);
            if (line.length() == 0)
                continue;
            lParser.parse(line);
            if(comands_.count(lParser.getComand()) == 1)
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

    LineParser lParser;

    void mkFile     ( ) {
         std::clog << "in mkFile func\n";
         curDir_.creatFile(lParser.getArgs()[0]);
    }
    void ls         ( ) {
        std::clog << "in ls func\n";
        curDir_.showCatalog();
    }

    void mkdir      ( ) { std::clog << "in mkdir func\n"; }
    void addUser    ( ) { std::clog << "in addUser func\n"; }
    void deleteUser ( ) { std::clog << "in deleteUser func\n"; }
    void cp         ( ) { std::clog << "in cp func\n"; }
    void rm         ( ) { std::clog << "in rm func\n"; }
    void exit       ( ) { std::clog << "in exit func\n"; }
    void showUsers  ( ) { std::clog << "in showUsers func\n"; }
    void cd         ( ) { std::clog << "in cd func\n"; }
    void mv         ( ) { std::clog << "in mv func\n"; }


    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
