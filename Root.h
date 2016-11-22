#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "User.h"
#include "Files/File.h"
#include "Files/Catalog.h"
#include "LineParser.h"
//#include "CommandArray.h"

class Root {
public: 
    Root() {
        comands_["mkdir"] = &Root::mkdir;
        comands_["adduser"] = &Root::addUser;
        comands_["deluser"] = &Root::deleteUser;
        comands_["cp"] = &Root::cp;
        comands_["rm"] = &Root::rm;
        comands_["exit"] = &Root::exit;
        comands_["showusers"] = &Root::showUsers;
        comands_["cd"] = &Root::cd;
        comands_["mv"] = &Root::mv;
    }

    typedef void(Root::*funcPtr)(void);

    void startWork(std::istream &is) {
        std::string line;
        std::getline(is, line);
        lParser.parse(line);
        int a = comands_.count(lParser.getComand());
        if(comands_.count(lParser.getComand()) == 1)
            (this->*comands_[lParser.getComand()])();
        else
            std::cout << "No command '" << lParser.getComand() << "' found" << std::endl;
    }
private: 
    User curUser_;
//    Catalog rootDir_; не удалять
//    Catalog& curDir_; не удалять
    std::vector<User> users_;
    std::vector<UserGroup> groups_;
    std::map<std::string,funcPtr> comands_;

    LineParser lParser;


    void mkdir      ( ) { qDebug() << "in mkdir func"; }
    void addUser    ( ) { qDebug() << "in addUser func"; }
    void deleteUser ( ) { qDebug() << "in deleteUser func"; }
    void cp         ( ) { qDebug() << "in cp func"; }
    void rm         ( ) { qDebug() << "in rm func"; }
    void exit       ( ) { qDebug() << "in exit func"; }
    void showUsers  ( ) { qDebug() << "in showUsers func"; }
    void cd         ( ) { qDebug() << "in cd func"; }
    void mv         ( ) { qDebug() << "in mv func"; }


    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
