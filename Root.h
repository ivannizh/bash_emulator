#ifndef _ROOT_H
#define _ROOT_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <fstream>

#include "UserControl.h"
#include "Files/Catalog.h"
#include "LineParser.h"

class Root {
public:
    Root();

    static std::ofstream f;
    typedef void(Root::*funcPtr)();

    void startWork();

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
    void unBlockW      ( );
    void unBlockR      ( );
    void chowner       ( );
    void addUser       ( );
    void logOut        ( );
    void mkFile        ( );
    void blockW        ( );
    void blockR        ( );
    void mkdir         ( );
    void chmod         ( );
    void clean         ( );
    void write         ( );
    void exit          ( );
    void pwd           ( );
    void cat           ( );
    void ls            ( );
    void cd            ( );
    void rm            ( );
    void mv            ( );
    void cp            ( );
};

#endif //_ROOT_H
