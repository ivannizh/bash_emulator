#include "Root.h"

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
    curDir_.showCatalog(uControl);
}
