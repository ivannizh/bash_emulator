#include "Descriptor.h"

void Descriptor::showInfo() const {
    std::cout << permissoin_ << " " << created_ << " ";
}

int Descriptor::getOwner() const {
    return permissoin_.userId();
}

int Descriptor::getGroup() const {
    return permissoin_.groupId();
}

Permission &Descriptor::perm(){
    return permissoin_;
}
