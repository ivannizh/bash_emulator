#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <typeinfo>

#include "UserControl.h"
#include "Files/Catalog.h"
#include "LineParser.h"

//TODO : попробовать использовать . для получения имени

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
    int curUserId_;
    Catalog rootDir_;
    Catalog* curDir_;

    std::map<std::string,funcPtr> comands_;
    enum descrType {FILE, CATALOG};

    LineParser lParser;

    void mkFile ( ) { Root::newDescriptor(descrType::FILE);   }
    void mkdir  ( ) { Root::newDescriptor(descrType::CATALOG); }

    void newDescriptor (descrType t);
    void deleteGroup   ( );
    void showGroups    ( );
    void deleteUser    ( );
    void showUsers     ( );
    void addUser       ( );
    void logOut        ( );
    void exit          ( );
    void pwd           ( );
    void ls            ( );
    void cd            ( );


    void chmod ( ) {
        if(lParser.getArgsSize() == 0){
            std::cout << "Missing file" << std::endl;
        }

        std::string fName = lParser.getArgs()[0];

        Descriptor* descr = curDir_->getFile(fName);

        if (descr->getOwner() == curUserId_ || uControl_.isUserInGroup(curUserId_, descr->getGroup()) ||
                uControl_.isUserInGroup(curUserId_, 2)){

            const LineParser::Param params = lParser.getParams();

            for(const auto& param: params){

                if(param.first == "u" || param.first == "g" || param.first == "o") {
                    descr->perm() = param.first + param.second;
                } else if(param.first == "u+" || param.first == "g+" || param.first == "o+") {
                    descr->perm() += param.first[0] + param.second;
                } else if(param.first == "u-" || param.first == "g-" || param.first == "o-") {
                    descr->perm() -= param.first[0] + param.second;
                }
            }
        }
    }
    
    void mv              ( ) {} //TODO
    void cp         ( ) {} //TODO
    void rm         ( ) {} //TODO

//    void changeUser      ( ) {}
//    void changeUSerTable ( ) {}
//    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
