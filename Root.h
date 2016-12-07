#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <typeinfo>

#include "UserControl.h"
#include "Files/Catalog.h"
#include "LineParser.h"

class Root {
public:
    Root();

    typedef void(Root::*funcPtr)();

    void startWork(std::istream &is) {
        std::string line;
        while(true){
            if(!curUserId_){
                std::cout << "Login: ";
                std::string name;
                std::cin >> name;
                int id = uControl_.getUserIdByName(name);
                if(!id) {
                    std::cout << "Incorrect user name" << std::endl;
                    continue;
                }
                std::cout << "Welcome " << name << std::endl;
                curUserId_ = id;
                std::cin.ignore();
            } else {
                std::cout << "\033[1;32m" << uControl_.getUserNameById(curUserId_) << ":" << "\033[0m"
                          << "\033[0;36m" << *curDir_ << "\033[0m" << "$";
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
    }

private:
    UserControl uControl_;
    int curUserId_;
    Catalog rootDir_;
    Catalog* curDir_;

    std::map<std::string,funcPtr> comands_;
    enum descrType {FILE, CATALOG};

    LineParser lParser;

    void mkFile ( ) { Root::newDescriptor(descrType::FILE);   }
    void mkdir  ( ) { Root::newDescriptor(descrType::CATALOG); }

    void newDescriptor (descrType t);
    void ls            ( );
    void addUser       ( );
    void deleteUser    ( );
    void deleteGroup   ( );
    void showUsers     ( );
    void showGroups    ( );
    void cd            ( );

    void chmod ( ) { }
    
    void mv              ( ) {}
    void cp         ( ) {}
    void rm         ( ) {}
    void exit       ( ) { std::exit(0); }
    void logOut ();

    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
