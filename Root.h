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
            } else {
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

    void newDescriptor(descrType t);

    void ls ( );

    void addUser ( ) {
        std::string group = lParser.getParam("g");
        std::vector<std::string> names  = lParser.getArgs();


        for(const std::string name: names)
            uControl_.addUser(name, group);
    }
    
    void chmod ( ) { }
    
    void deleteUser ( ) {
        std::vector<std::string> names = lParser.getArgs();

        for(const std::string &name: names)
            uControl_.deleteUser(name);
    }
    void deleteGroup ( ) {
        std::vector<std::string> names = lParser.getArgs();

        for(const std::string &name: names)
            uControl_.deleteGroup(name);
    }
    void cp         ( ) {}
    void rm         ( ) {}
    void exit       ( ) {
        std::exit(0);
    }
    void logOut () {
        std::cout << "Good bye" << std::endl;
        curUserId_ = 0;
    }

    void showUsers  ( ) {
        bool showGroups = lParser.getParam("g") == "\n";

        uControl_.showUsers(showGroups);
    }
    void showGroups  ( ) {
        bool showUsers = lParser.getParam("u") == "\n";

        uControl_.showGroups(showUsers);
    }
    void cd         ( ) {
        Catalog* cat = curDir_->getCatalog(lParser.getArgs()[0]);
        if(cat == curDir_ || cat == nullptr)
            std::cout << "No such file was found" << std::endl;
        else
            curDir_ = cat;


    }
    void mv         ( ) {}
    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
