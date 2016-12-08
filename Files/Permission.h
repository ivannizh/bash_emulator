#ifndef _PERMISSION_H
#define _PERMISSION_H

#include <vector>
#include <iostream>

#include "../UserControl.h"
//
// d rwx
// 1 421

class Permission {
public: //TODO
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

    bool checkWrite (int user) { return checkPerm(4, user); }
    bool checkRead  (int user) { return checkPerm(3, user); }
    bool checkX     (int user) { return checkPerm(3, user); }
    
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
    const UserControl& uControl_;
    bool d_;
    int userId_;
    int groupId_;
    std::vector<char> p_;

    bool checkPerm(int type, int user){
        //TODO
        if (user == userId_)
            return p_[0] & type;
        if (uControl_.isUserInGroup(user, groupId_))
            return p_[1] & type;

        return p_[2] & type;
    }
};

#endif //_PERMISSION_H
