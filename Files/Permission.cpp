#include "Permission.h"

Permission::Permission(const UserControl &uCtrl) : d_(1), uControl_(uCtrl), userId_(1),
    groupId_(uControl_.getGroupIdByName(uControl_.getUserNameById(1))){
    p_.push_back(7);
    p_.push_back(7);
    p_.push_back(5);
}

Permission::Permission(int user, bool isDrive, const UserControl &uCtrl) : d_(isDrive), uControl_(uCtrl), userId_(user),
    groupId_(uControl_.getGroupIdByName(uControl_.getUserNameById(user))) {
    if (isDrive){
        p_.push_back(7);
        p_.push_back(7);
        p_.push_back(5);
    } else {
        p_.push_back(6);
        p_.push_back(6);
        p_.push_back(4);
    }
}

void Permission::newUser(int user) { userId_ = user; }

void Permission::newGroup(int group) {groupId_ = group; }

Permission &Permission::operator+=(const std::__cxx11::string &perm) {
    forOperators(perm, [](char& p, const char& t) -> void { p = p |  t; });
    return *this;
}

Permission& Permission::operator-=(const std::__cxx11::string &perm) {
    forOperators(perm, [](char& p, const char& t) -> void { p = p & ~t; });
    return *this;
}

Permission& Permission::operator=(const std::__cxx11::string &perm) {
    forOperators(perm, [](char& p, const char& t) -> void { p =      t; });
    return *this;
}

void Permission::changePerm(int perm) {
    if(perm < 100 || perm > 777){
        std::cout << "Wrong permision value" << std::endl;
        return;
    }

    for(char& it: p_){
        it = perm%10;
        perm /= 10;
    }
}

std::ostream& operator<< (std::ostream &os, const Permission &perm){
    perm.d_ ? os<<'d' : os<<'-';
    for(char it: perm.p_){
        it & 4 ? os<<'r' : os<<'-';
        it & 2 ? os<<'w' : os<<'-';
        it & 1 ? os<<'x' : os<<'-';
    }
    os << " " << perm.userName() << " " << perm.groupName();
    return os;
}

int Permission::userId() const
{
    return userId_;
}

int Permission::groupId() const
{
    return groupId_;
}

std::__cxx11::string Permission::userName() const {
    std::string name = uControl_.getUserNameById(userId_);
    return name;
}

std::__cxx11::string Permission::groupName() const {
    return uControl_.getGroupNameById(groupId_);
}

const UserControl &Permission::uControl() const
{
    return uControl_;
}

void Permission::forOperators(const std::__cxx11::string &q, void (*func)(char &, const char &)){
    char type = 0;
    for(size_t i = 1; i < q.size(); ++i){
        switch (q[i]) {
        case 'r': type += 4; break;
        case 'w': type += 2; break;
        case 'x': type += 1; break;
        }
    }
    switch (q[0]) {
    case 'u': func(p_[0], type); break;
    case 'g': func(p_[1], type); break;
    case 'o': func(p_[2], type); break;
    }
}

bool Permission::checkPerm(int type, int user){

    if (user == userId_)
        return p_[0] & type;
    if (uControl_.isUserInGroup(user, groupId_))
        return p_[1] & type;

    return p_[2] & type;
}
