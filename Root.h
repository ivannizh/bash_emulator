#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include "UserControl.h"
#include "Files/Catalog.h"
#include "LineParser.h"

class Root {
public:
    Root();

    typedef void(Root::*funcPtr)();

    void startWork() {
        std::string line;
        while(true){
            if(!curUserId_){
                std::cout << "Login: ";
                std::string name;
                std::cin >> name;
                int id = uControl_.getUserIdByName(name);
                if(std::cin.eof()){
                    std::cout << "\nGoodbye" << std::endl;
                    return;
                }
                if(!id) {
                    std::cout << "Incorrect user name" << std::endl;
                    continue;
                }
                std::cout << "Welcome " << name << std::endl;
                curUserId_ = id;
                std::cin.ignore();

            } else {
                std::cout << "\033[1;32m" << uControl_.getUserNameById(curUserId_) << ":" << "\033[0m"
                          << "\033[0;36m" << *curDir_ << "\033[0m" << "$ ";
                line = "";
                std::getline(std::cin, line);
                if(std::cin.eof()){
                    std::cout << "\nGoodbye" << std::endl;
                    return;
                }
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
    Catalog rootDir_;
    Catalog* curDir_;
    int curUserId_;
    std::map<std::string,funcPtr> comands_;
    enum descrType {FILE, CATALOG};
    LineParser lParser;

    void cd     (std::string dir);

    void newDescriptor (descrType t, const std::__cxx11::string &fName);
    void deleteGroup   ( );
    void showGroups    ( );
    void deleteUser    ( );
    void showUsers     ( );
    void chowner       ( );
    void addUser       ( );
    void logOut        ( );
    void mkFile        ( );
    void mkdir         ( );
    void chmod         ( );
    void exit          ( );
    void pwd           ( );
    void ls            ( );
    void cd            ( );
    void rm            ( );


    void mv            ( ) {} //TODO
    void cp            ( ) {} //TODO
};

#endif //_ROOT_H
