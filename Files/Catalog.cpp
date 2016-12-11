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

bool Catalog::checkX(int user) const {
    return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkX(user);
}

bool Catalog::checkW(int user) const {
    return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
}

bool Catalog::checkR(int user) const {
    return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkR(user);
}

void Catalog::delRef(const std::__cxx11::string &fName){
    fTable_.delRef(fName);
}

void Catalog::addRef(const std::__cxx11::string &fName, Descriptor *descr){
    fTable_.addRef(fName, descr);
}

Catalog* Catalog::getCatalog(const std::__cxx11::string &name) {
    try                          { return dynamic_cast<Catalog*>(getDescr(name)); }
    catch (const std::bad_cast&) { return dynamic_cast<Catalog*>(fTable_.getCurDir());   }
}

File* Catalog::getFile(const std::__cxx11::string &name) {
    try                          { return dynamic_cast<File*>(getDescr(name));      }
    catch (const std::bad_cast&) { return nullptr; }
}

Descriptor *Catalog::getDescr(const std::__cxx11::string &name) {
    return fTable_.getFile(name);
}

int Catalog::getOwner(const std::__cxx11::string &fName) {
    return fTable_.getOwner(fName);
}

int Catalog::getGroup(const std::__cxx11::string &fName) {
    return fTable_.getGroup(fName);
}

void Catalog::deleteItSelf(int user)  throw (Errors::PermissionDenied){
    if (!perm().checkX(user) || !perm().checkW(user))
        throw Errors::PermissionDenied();
    fTable_.deleteTable(user);
}

void Catalog::chowner(const std::__cxx11::string &fName,
                      const std::__cxx11::string &newUser,
                      const std::__cxx11::string &newGroup, int user)
                      throw (Errors::PermissionDenied) {
    int userId  = permissoin_.uControl().getUserIdByName  ( newUser  );
    int groupId = permissoin_.uControl().getGroupIdByName ( newGroup );

    if(userId == 0 && groupId == 0) {
        std::cout << "Wrong user or group name" << std::endl;
        return;
    }

    Descriptor* file = fTable_.getFile(fName);

    if (file == this){
        Errors::noFile(fName);
    }

    if(permissoin_.uControl().isUserInGroup(user, 2) ||
            permissoin_.uControl().isUserInGroup(user, file->perm().groupId()) ||
            file->getOwner() == user){
        if (userId  != 0)
            file->perm().newUser  ( userId  );
        if (groupId != 0)
            file->perm().newGroup ( groupId );

    } else {
        Errors::PermissionDenied::printError();
        return;
    }
}

Descriptor *Catalog::retCopy(const std::__cxx11::string &fName, Catalog *parent){
    Descriptor* file = fTable_.getFile(fName);
    Descriptor* ret;

    if(file->perm().isDir()){
        ret = dynamic_cast<Catalog*>(file)->getCopy(parent);
    }else {
        ret = dynamic_cast<File*>(file)->getCopy();
    }

    return ret;
}

Catalog *Catalog::getCopy(Catalog *parent){
    Catalog* cat = new Catalog(permissoin_, parent);

    for(const FilesTable::fileDescr& file: fTable_.getFiles()){

        if(file.first == "." || file.first == "..")
            continue;

        if(file.second->perm().isDir()){
            cat->addRef(file.first, dynamic_cast<Catalog*>(file.second)->getCopy(cat));
        } else {
            cat->addRef(file.first, dynamic_cast<File*>(file.second)->getCopy());
        }

    }
    return cat;
}

bool Catalog::checkParentWritePerm(int user){
    return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
}

std::ostream& operator<<(std::ostream &os, const Catalog &cat){
    os << (dynamic_cast<Catalog*>(cat.fTable_.getParent())->getDirName(&cat));
    return os;
}
