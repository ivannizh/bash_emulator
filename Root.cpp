#include "Root.h"

Root::Root() : uControl_(), rootDir_(1, uControl_), curDir_(&rootDir_), curUserId_(0){

    comands_["logout"]    = &Root::logOut;
    comands_["touch"]     = &Root::mkFile;
    comands_["mkdir"]     = &Root::mkdir;
    comands_["chmod"]     = &Root::chmod;
    comands_["addu"]      = &Root::addUser;
    comands_["delu"]      = &Root::deleteUser;
    comands_["exit"]      = &Root::logOut   ;
    comands_["delg"]      = &Root::deleteGroup;
    comands_["shu"]       = &Root::showUsers;
    comands_["shg"]       = &Root::showGroups;
    comands_["pwd"]       = &Root::pwd;
    comands_["cp"]        = &Root::cp;
    comands_["rm"]        = &Root::rm;
    comands_["cd"]        = &Root::cd;
    comands_["mv"]        = &Root::mv;
    comands_["ls"]        = &Root::ls;
    comands_["lo"]        = &Root::logOut;
    comands_["e"]         = &Root::exit;
    comands_[">"]         = &Root::mkFile;
}

void Root::newDescriptor(Root::descrType t){
    if (lParser.getParamsSize() > 0)
        std::cout << "Ignoring parametres" << std::endl;

    for(const auto name: lParser.getArgs())
        switch (t) {
        case descrType::CATALOG:
            curDir_->creatCatalog(name, curUserId_);
            break;
        case descrType::FILE:
            curDir_->creatFile(name, curUserId_);
            break;
        default:
//            std::cerr << "ERROR in func Root::newDescriptor" << std::endl;
            break;
        }
}

void Root::ls() {
    curDir_->showCatalog();
}

void Root::addUser() {
    std::string group = lParser.getParam("g");
    std::vector<std::string> names  = lParser.getArgs();


    for(const std::string name: names)
        uControl_.addUser(name, group);
}

void Root::deleteUser() {
    std::vector<std::string> names = lParser.getArgs();

    for(const std::string &name: names)
        uControl_.deleteUser(name);
}

void Root::deleteGroup() {
    std::vector<std::string> names = lParser.getArgs();

    for(const std::string &name: names)
        uControl_.deleteGroup(name);
}

void Root::logOut() {
    std::cout << "Good bye" << std::endl;
    curUserId_ = 0;
}

void Root::exit() {std::exit(0); }

void Root::showUsers() {
    bool showGroups = lParser.getParam("g") == "\n";

    uControl_.showUsers(showGroups);
}

void Root::showGroups() {
    bool showUsers = lParser.getParam("u") == "\n";

    uControl_.showGroups(showUsers);
}

void Root::cd() {
    Catalog* cat = curDir_->getCatalog(lParser.getArgs()[0]);
    if(cat == curDir_ || cat == nullptr)
        std::cout << "No such directory was found" << std::endl;
    else {
        if(cat->checkX(curUserId_))
            curDir_ = cat;
        else
            std::cout << "\033[31m" << "Permission denied" << "\033[0m" << std::endl;
    }


}

void Root::pwd(){ std::cout << *curDir_ << std::endl; }
