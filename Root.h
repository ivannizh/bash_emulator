#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "UserControl.h"
//#include "Files/File.h"
#include "Files/Catalog.h"
#include "LineParser.h"
//#include "CommandArray.h"

class Root {
public:
    Root() : rootDir_(1), curDir_(rootDir_), curUserId_(0){

        comands_["touch"]     = &Root::mkFile;
        comands_["mkdir"]     = &Root::mkdir;
        comands_["logout"]    = &Root::logOut;
        comands_["addu"]      = &Root::addUser;
        comands_["delu"]      = &Root::deleteUser;
        comands_["exit"]      = &Root::exit;
        comands_["delg"]      = &Root::deleteGroup;
        comands_["shu"]       = &Root::showUsers;
        comands_["shg"]       = &Root::showGroups;
        comands_["cp"]        = &Root::cp;
        comands_["rm"]        = &Root::rm;
        comands_["cd"]        = &Root::cd;
        comands_["mv"]        = &Root::mv;
        comands_["ls"]        = &Root::ls;
        comands_["lo"]        = &Root::logOut;
        comands_["e"]         = &Root::exit;
        comands_[">"]         = &Root::mkFile;
    }

    typedef void(Root::*funcPtr)();

    void startWork(std::istream &is) {
        std::string line;
        while(true){
            if(!curUserId_){
                std::cout << "Login: ";
                std::string name;
                std::cin >> name;
                int id = uControl.getUserIdByName(name);
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
    int curUserId_;
    Catalog rootDir_; // не удалять
    Catalog& curDir_; // не удалять
    UserControl uControl;

    std::map<std::string,funcPtr> comands_;
    enum descrType {FILE, CATALOG};

    LineParser lParser;

    void mkFile () { newDescriptor(descrType::FILE);    }
    void mkdir  () { newDescriptor(descrType::CATALOG); }

    void newDescriptor(descrType t){
        if (lParser.getParamsSize() > 0)
            std::cout << "Ignoring parametres" << std::endl;

        for(const auto name: lParser.getArgs())
            switch (t) {
            case descrType::CATALOG:
                curDir_.creatCatalog(name, curUserId_);
                break;
            case descrType::FILE:
                curDir_.creatFile(name, curUserId_);
                break;
            default:
                std::cerr << "ERROR in func Root::newDescriptor" << std::endl;
                break;
            }
    }

    void ls ( ) {
        curDir_.showCatalog();
    }

    void addUser    ( ) {
        std::string group = lParser.getParam("g");
        std::vector<std::string> names  = lParser.getArgs();

        if (lParser.getParamsSize() > 0){
            std::cout << "Ignoring other parametres" << std::endl;
        }

        for(const std::string name: names)
            uControl.addUser(name, group);
    }
    void deleteUser ( ) {
        std::vector<std::string> names = lParser.getArgs();

        if (lParser.getParamsSize() > 0)
            std::cout << "Ignoring parametres" << std::endl;
        for(const std::string &name: names)
            uControl.deleteUser(name);
    }
    void deleteGroup ( ) {
        std::vector<std::string> names = lParser.getArgs();

        if (lParser.getParamsSize() > 0)
            std::cout << "Ignoring parametres" << std::endl;
        for(const std::string &name: names)
            uControl.deleteGroup(name);
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

        if (lParser.getArgsSize() > 0)
            std::cout << "Ignoring arguments" << std::endl;
        if (lParser.getParamsSize() > 0)
            std::cout << "Ignoring parametres" << std::endl;

        uControl.showUsers(showGroups);
    }
    void showGroups  ( ) {
        bool showUsers = lParser.getParam("u") == "\n";

        if (lParser.getArgsSize() > 0)
            std::cout << "Ignoring arguments" << std::endl;
        if (lParser.getParamsSize() > 0)
            std::cout << "Ignoring parametres" << std::endl;

        uControl.showGroups(showUsers);
    }
    void cd         ( ) {}
    void mv         ( ) {}


    void changeUser      ( ) {}
    void changeUSerTable ( ) {}
    void showStatistic   ( ) const {}
};

#endif //_ROOT_H
