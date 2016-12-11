#include "Catalog.h"

void Catalog::creatFile(const std::__cxx11::string &name, int user) {
    if(checkParentWritePerm(user))
        fTable_.addDescr(new File(user, permissoin_.uControl()), name);
    else
        std::cout << "\033[31m" << "Permission denied" << "\033[0m" << std::endl;
}

void Catalog::creatCatalog(const std::__cxx11::string &name, int user) {
    if(checkParentWritePerm(user))
        fTable_.addDescr(new Catalog(user, this, permissoin_.uControl()), name);
    else
        Errors::PermissionDenied::printError();
}

std::__cxx11::string Catalog::getDirName(const Catalog *cat){
    if (cat == this) return "/";

    Catalog* parent = dynamic_cast<Catalog*>(fTable_.getFile(".."));
    const std::string catName = fTable_.getName(cat);

    if(parent == this) return "/" + catName;
    else               return parent->getDirName(this) + "/" + catName;
}

void Catalog::showCatalog(std::__cxx11::string dir, int user) const throw(std::invalid_argument){
    if(dir == ""){
        fTable_.showTable();
        return;
    }
    int npos = dir.find("/");
    std::string nextDir = dir.substr(0, npos);
    if(npos < 0)
        dir.clear();
    else
        dir.erase(0, npos+1);

    Catalog* cat = dynamic_cast<Catalog*>(fTable_.getFile(nextDir));
    if(cat == this)
        throw std::invalid_argument("");

    if (cat->checkR(user)){
        try {
            cat->showCatalog(dir, user);
        } catch (const std::invalid_argument&) {
            throw;
        }
    } else {
        Errors::PermissionDenied::printError();
    }
}

void Catalog::deleteFile(const std::__cxx11::string &file, bool isRec, const int user) throw (Errors::PermissionDenied){

    if(fTable_.getFile(file) == this) {
        Errors::noFile(file);
        return;
    }

    Descriptor* descr = fTable_.getFile(file);
    if(perm().checkW(user) || descr->perm().userId() == user
            || permissoin_.uControl().isUserInGroup(user,descr->perm().groupId())) {
        if (descr->perm().isDir()){
            if(!isRec){
                std::cout << "cannot remove '" << file << "': Is a directory" << std::endl;
                return;
            }
            try {
                dynamic_cast<Catalog*>(descr)->fTable_.deleteTable(user);
                fTable_.deleteFile(file);
            } catch(const Errors::PermissionDenied&) {
                throw;
            }
        } else {
            fTable_.deleteFile(file);
        }
    } else
        throw Errors::PermissionDenied();
}

bool Catalog::checkParentWritePerm(int user){
    return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
}

std::ostream& operator<<(std::ostream &os, const Catalog &cat){
    os << (dynamic_cast<Catalog*>(cat.fTable_.getParent())->getDirName(&cat));
    return os;
}
