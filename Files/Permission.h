#ifndef _PERMISSION_H
#define _PERMISSION_H

#include <vector>
#include <iostream>

#include "../UserControl.h"

// d rwx
// 1 421

class Permission {
public:
    explicit Permission ( const UserControl& uCtrl) : d_(1), uControl_(uCtrl), userId_(1),
        groupId_(uControl_.getGroupIdByName(uControl_.getUserNameById(1))){
        p_.push_back(7);
        p_.push_back(7);
        p_.push_back(5);
    }

    Permission ( int user, bool isDrive, const UserControl& uCtrl) : d_(isDrive), uControl_(uCtrl), userId_(user),
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

    bool checkR (int user) { return checkPerm(4, user); }
    bool checkW (int user) { return checkPerm(2, user); }
    bool checkX (int user) { return checkPerm(1, user); }

    Permission& operator+= (const std::string& perm) {
        forOperators(perm, [](char& p, const char& t) -> void { p = p |  t; });
        return *this;
    }
    Permission& operator-= (const std::string& perm) {
        forOperators(perm, [](char& p, const char& t) -> void { p = p & ~t; });
        return *this;
    }
    Permission& operator= (const std::string& perm) {
        forOperators(perm, [](char& p, const char& t) -> void { p =      t; });
        return *this;
    }
    
    void changePerm ( int perm ) {
       if(perm < 100 || perm > 777){
           std::cout << "Wrong permision value" << std::endl;
           return;
       }

       for(char it: p_){
           it = perm%10;
           perm /= 10;
       }
    }

    friend std::ostream& operator<< ( std::ostream& os, const Permission &perm ){
        perm.d_ ? os<<'d' : os<<'-';
        for(char it: perm.p_){
            it & 4 ? os<<'r' : os<<'-';
            it & 2 ? os<<'w' : os<<'-';
            it & 1 ? os<<'x' : os<<'-';
        }
        return os;
    }

    int userId() const;

    int groupId() const;

    std::string userName() const {
        std::string name = uControl_.getUserNameById(userId_);
        return name;
    }


    std::string groupName() const {
        return uControl_.getGroupNameById(groupId_);
    }

    const UserControl &uControl() const;

private:
    bool d_;
    const UserControl& uControl_;
    int userId_;
    int groupId_;
    std::vector<char> p_;

    void forOperators(const std::string& q, void (*func)(char&, const char&)){
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

    bool checkPerm(int type, int user){

        if (user == userId_)
            return p_[0] & type;
        if (uControl_.isUserInGroup(user, groupId_))
            return p_[1] & type;

        return p_[2] & type;
    }
};

#endif //_PERMISSION_H
