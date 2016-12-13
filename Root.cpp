#include "Root.h"

std::ofstream Root::f;

Root::Root() : uControl_(), rootDir_(uControl_), curDir_(&rootDir_), curUserId_(0){

    comands_["unBlockW"]  = &Root::unBlockW;
    comands_["unBlockR"]  = &Root::unBlockR;
    comands_["chowner"]   = &Root::chowner;
    comands_["logout"]    = &Root::logOut;
    comands_["blockW"]    = &Root::blockW;
    comands_["blockR"]    = &Root::blockR;
    comands_["touch"]     = &Root::mkFile;
    comands_["mkdir"]     = &Root::mkdir;
    comands_["chmod"]     = &Root::chmod;
    comands_["clean"]     = &Root::clean;
    comands_["write"]     = &Root::write;
    comands_["addu"]      = &Root::addUser;
    comands_["delu"]      = &Root::deleteUser;
    comands_["exit"]      = &Root::logOut;
    comands_["delg"]      = &Root::deleteGroup;
    comands_["shu"]       = &Root::showUsers;
    comands_["shg"]       = &Root::showGroups;
    comands_["pwd"]       = &Root::pwd;
    comands_["cat"]       = &Root::cat;
    comands_["cp"]        = &Root::cp;
    comands_["rm"]        = &Root::rm;
    comands_["cd"]        = &Root::cd;
    comands_["mv"]        = &Root::mv;
    comands_["ls"]        = &Root::ls;
    comands_["lo"]        = &Root::logOut;
    comands_["e"]         = &Root::exit;
    comands_[">"]         = &Root::mkFile;

    f.open("log.txt", std::ios::out | std::ios::app);

}

void Root::startWork() {
    std::string line;

    f << std::endl << std::endl << "new session " << Date() << std::endl;

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
            f << "User '" << name << "' was loged in" << std::endl << std::flush;
            curUserId_ = id;
            std::cin.ignore();

        } else {
            std::cout << "\033[1;32m" << uControl_.getUserNameById(curUserId_) << ":" << "\033[0m"
                      << "\033[0;36m" << *curDir_ << "\033[0m" << "$ ";
            line = "";
            std::getline(std::cin, line);
            f << line << std::endl << std::flush;

            if(std::cin.eof()){
                std::cout << "\nGoodbye" << std::endl;
                return;
            }
            if (line.length() == 0)
                continue;
            lParser.parse(line);
            if(comands_.count(lParser.getComand()))
                (this->*comands_[lParser.getComand()])  ();
            else
                std::cout << "No command '" << lParser.getComand() << "' found" << std::endl;
        }
    }
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
            Errors::noCatalog(tmpDir);
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

void Root::write() {
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkW(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't write in file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        std::string line;
        std::getline(std::cin, line);
        f->write(line, curUserId_);
    }

    curDir_ = tmp;
}

void Root::cat(){

    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkR(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't write in file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->cat(curUserId_);
    }

    curDir_ = tmp;

}

void Root::clean() {
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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

    Descriptor* descr = curDir_->getDescr(fName);

    if(descr->perm().isDir()){
        std::cout << "Can't write in file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->clean(curUserId_);
    }
    curDir_ = tmp;
}

void Root::blockW(){
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkW(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't block file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->makeUnWriteable(curUserId_);
    }

    curDir_ = tmp;
}

void Root::blockR() {
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkR(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't block file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->makeUnReadable(curUserId_);
    }

    curDir_ = tmp;
}

void Root::unBlockW(){
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkW(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't unblock file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->makeWriteable(curUserId_);
    }

    curDir_ = tmp;
}

void Root::unBlockR(){
    if(lParser.getArgsSize() < 1){
        std::cout << "Missing file" << std::endl;
        return;
    }

    std::string file = lParser.getArgs()[0];

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


    Descriptor* descr = curDir_->getDescr(fName);

    if(!descr->perm().checkR(curUserId_)){
        Errors::PermissionDenied::printError();
        curDir_ = tmp;
    }

    if(descr->perm().isDir()){
        std::cout << "Can't block file '" << fName << "'. It's a directory" << std::endl;
    } else {
        File* f = dynamic_cast<File*>(descr);
        f->makeReadable(curUserId_);
    }

    curDir_ = tmp;
}

void Root::mv() {

    if(lParser.getArgsSize() < 2){
        std::cout << "Missing destination" << std::endl;
        return;
    }

    std::vector<std::string> args = lParser.getArgs();
    std::string dest = args.back();
    args.erase(args.end() - 1);

    while(dest[dest.length() - 1] == '/')
        dest.erase(dest.length()-1, 1);

    Catalog* tmp = curDir_;
    Catalog* destDir;

    if (dest == ".")
        destDir = curDir_;
    else {
        cd(dest);
        destDir = curDir_;
        curDir_ = tmp;
    }

    if (!destDir->checkW(curUserId_)){
        std::cout << "Destination dir error: ";
        Errors::PermissionDenied::printError();
        return;
    }

    for(std::string file: args){
        while(file[file.length()-1] == '/')
            file.erase(file.length()-1, 1);

        Catalog* tmp = curDir_;
        int npos = file.find_last_of('/');
        std::string fName = file.substr(npos + 1, file.length());

        if(npos < 0)
            file = "";
        else {
            file.erase(npos, file.length());

            if (!(dest.find(file) > 0)){
                std::cout << "Sorry, can't move '" << fName << "' to '" << dest << "'" << std::endl;
                curDir_ = tmp;
                continue;
            }
            cd(file);
        }

        if (!curDir_->checkW(curUserId_)){
            curDir_ = tmp;
            std::cout << "Can't move file '" << fName << "' : ";
            Errors::PermissionDenied::printError();
            continue;
        }

        File* fileRef = curDir_->getFile(fName);
        curDir_->delRef(fName);

        destDir->addRef(fName, fileRef);

        curDir_ = tmp;
    }

}

void Root::cp() {

    if(lParser.getArgsSize() < 2){
        std::cout << "Missing destination" << std::endl;
        return;
    }

    std::vector<std::string> args = lParser.getArgs();
    std::string dest = args.back();
    args.erase(args.end() - 1);

    while(dest[dest.length() - 1] == '/')
        dest.erase(dest.length()-1, 1);

    Catalog* tmp = curDir_;
    Catalog* destDir;

    if (dest == ".")
        destDir = curDir_;
    else {
        cd(dest);
        destDir = curDir_;
        curDir_ = tmp;
    }

    if (!destDir->checkW(curUserId_)){
        std::cout << "Destination dir error: ";
        Errors::PermissionDenied::printError();
        return;
    }

    for(std::string file: args){
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

        if (!curDir_->checkW(curUserId_)){
            curDir_ = tmp;
            std::cout << "Can't move file '" << fName << "' : ";
            Errors::PermissionDenied::printError();
            continue;
        }

        Descriptor* descr = curDir_->getDescr(fName);

        if(descr->perm().isDir())
            if(lParser.getParam("-r") != ""){
                std::cout << "omitting '" << fName << "', is a directory. Use -r" << std::endl;
                curDir_ = tmp;
                continue;
            }

        Descriptor* fileRef = curDir_->retCopy(fName, destDir);

        destDir->addRef(fName, fileRef);

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
