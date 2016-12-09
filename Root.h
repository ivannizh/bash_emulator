#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>
//#include <typeinfo>

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
                std::cout << "'" << name << "'";
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
    void chmod         ( );
    void exit          ( );
    void pwd           ( );
    void ls            ( );
    void cd            ( );


    void cd (std::string dir);
    void rm         ( ) {
//        std::string file= lParser.getArgs()[0];
        for(std::string file: lParser.getArgs()){
            while(file[file.length()-1] == '/')
                file.erase(file.length()-1, 1);

            Catalog* tmp = curDir_;
            int npos = file.find_last_of('/');
            std::string fName = file.substr(npos + 1, file.length());

            if(npos < 0)
                file = "";
            else {
                file.erase(npos, file.length());
                cd(file);
            }
            try{
                curDir_->deleteFile(fName, lParser.getParam("r") == "\n", curUserId_);
            } catch (const Permission::PermissionDenied&) {
                std::cout  << "\033[31m" << "Permission denied : " << "\033[0m" << std::endl;
            }

            curDir_ = tmp;
        }
    }
    

    void chowner() {} //TODO
    void chgroup() {} //TODO
    void chother() {} // TODO

    void mv              ( ) {} //TODO
    void cp         ( ) {} //TODO

//    void changeUser      ( ) {}
//    void changeUSerTable ( ) {}
//    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
