#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "Permission.h"
#include "../UserControl.h"

class Descriptor {
public:
    explicit Descriptor (const UserControl &uCtrl) : permissoin_(uCtrl) { }

    Descriptor ( int userId, bool isDrive, const UserControl& uCtrl) : permissoin_(userId, isDrive, uCtrl) { }
    void showInfo() const {
        std::cout << permissoin_ << " " << created_ << " ";
    }
//    void open(int mod ){}

    virtual void deleteItSelf(int user) throw (Errors::PermissionDenied) = 0;

    int getOwner() const {
        return permissoin_.userId();
    }

    int getGroup() const {
        return permissoin_.groupId();
    }

    Permission& perm(){
        return permissoin_;
    }

    virtual ~Descriptor() {}

protected: 
//    const UserControl& uControl_;
//    int userId_;
//    int groupId_;
    Date created_;
    Date changed_;
    Permission permissoin_;
//    std::string pointer_; // ?
};

#endif //_DESCRIPTOR_H
