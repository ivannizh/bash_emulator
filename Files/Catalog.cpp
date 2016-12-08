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
        std::cout << "\033[31m" << "Permission denied" << "\033[0m" << std::endl;
}

std::__cxx11::string Catalog::getDirName(const Catalog *cat){
    if (cat == this) return "/";

    Catalog* parent = dynamic_cast<Catalog*>(fTable_.getFile(".."));
    const std::string catName = fTable_.getName(cat);

    if(parent == this) return "/" + catName;
    else               return parent->getDirName(this) + "/" + catName;
}

std::ostream& operator<<(std::ostream &os, const Catalog &cat){
    Catalog* parent = dynamic_cast<Catalog*>(cat.fTable_.getFile(".."));

    os << (parent->getDirName(&cat));
    return os;
}
