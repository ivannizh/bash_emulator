#include "Root.h"

Root::Root() : uControl_(), rootDir_(uControl_), curDir_(&rootDir_), curUserId_(0){

    comands_["chowner"]   = &Root::chowner;
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

void Root::mkFile() {
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
        Root::newDescriptor(descrType::FILE, fName);
        curDir_ = tmp;
    }
}

void Root::mkdir() {
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
        Root::newDescriptor(descrType::CATALOG, fName);
        curDir_ = tmp;
    }

}

void Root::newDescriptor(Root::descrType t, const std::string& fName){
    switch (t) {
    case descrType::CATALOG:
        curDir_->creatCatalog(fName, curUserId_);
        break;
    case descrType::FILE:
        curDir_->creatFile(fName, curUserId_);
        break;
    default:
        //            std::cerr << "ERROR in func Root::newDescriptor" << std::endl;
        break;
    }
}

void Root::ls() {
    std::string line = "";
    if (lParser.getArgsSize() > 0)
        line = lParser.getArgs()[0];
    try {
        curDir_->showCatalog(line, curUserId_);
    } catch (std::invalid_argument) {
        Errors::noFile(line);
    }

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

void Root::cd(){
    if(lParser.getArgsSize() == 0) {
        Errors::missedFiles();
        return;
    }
    Root::cd(lParser.getArgs()[0]);
}

void Root::cd(std::string dir) {
    std::string tmpDir = "";
    int npos = 0;

    while (dir[dir.length()-1] == '/')
        dir.erase(dir.length()-1, 1);

    do {
        npos = dir.find("/");
        tmpDir = dir.substr(0, npos);

        if (npos < 0)
            dir.clear();
        else
            dir.erase(0, npos + 1);

        Catalog* cat = curDir_->getCatalog(tmpDir);

        if(cat == curDir_ || cat == nullptr){
            Errors::noFile(tmpDir);
            return;
        }
        else {
            if(cat->checkX(curUserId_))
                curDir_ = cat;
            else {
                Errors::PermissionDenied::printError();
                return;
            }
        }
    } while (dir != "");
}

void Root::rm() {
    if(lParser.getArgsSize() == 0) {
        Errors::missedFiles();
        return;
    }
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
        } catch (const Errors::PermissionDenied&) {
            Errors::PermissionDenied::printError();
        }

        curDir_ = tmp;
    }
}

void Root::chowner() {
    if(lParser.getArgsSize() == 0) {
        Errors::missedFiles();
        return;
    }

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
        curDir_->chowner(fName, lParser.getParam("u"), lParser.getParam("g"), curUserId_);
        curDir_ = tmp;
    }
}

void Root::chmod() {
    if(lParser.getArgsSize() == 0){
        Errors::missedFiles();
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

void Root::pwd(){ std::cout << *curDir_ << std::endl; }
