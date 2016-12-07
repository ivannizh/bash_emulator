#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "Permission.h"
#include "../UserControl.h"

class Descriptor {
public:
    Descriptor ( int userId, bool isDrive, const UserControl& uCtrl) : userId_(userId), uControl_(uCtrl),
        groupId_(uControl_.getGroupIdByName(uControl_.getUserNameById(userId))),
        permissoin_(isDrive) {}

//    virtual Descriptor* makeCopy() = 0;

    void changePerm ( const std::string fileName, int perm ) { permissoin_.changePerm(perm); }
    void showInfo ( ) const {
        std::cout << permissoin_ << "  " << uControl_.getUserNameById(userId_) << "  " << uControl_.getGroupNameById(groupId_) << "  ";
    }
    void open ( int mod ) {}
    void deleteFile ( ) {}

    virtual ~Descriptor ( ) {}

protected: 
    const UserControl& uControl_;
    int userId_;
    int groupId_;
    Date created_;
    Date changed_;
    Permission permissoin_;
//    std::string pointer_; // ?
};

#endif //_DESCRIPTOR_H
