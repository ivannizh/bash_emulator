#include "Root.h"

Root::Root() : uControl_(), rootDir_(1, uControl_), curDir_(rootDir_), curUserId_(0){

    comands_["logout"]    = &Root::logOut;
    comands_["touch"]     = &Root::mkFile;
    comands_["mkdir"]     = &Root::mkdir;
    comands_["chmod"]     = &Root::chmod;
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

void Root::newDescriptor(Root::descrType t){
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

void Root::ls() {
    curDir_.showCatalog();
}
