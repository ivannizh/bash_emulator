#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "Permission.h"
#include "../UserControl.h"

class Descriptor {
public:
    explicit Descriptor (const UserControl &uCtrl):
        permissoin_(uCtrl) {

    }

    Descriptor ( int userId, bool isDrive, const UserControl& uCtrl) :
        permissoin_(userId, isDrive, uCtrl) {}

//    virtual Descriptor* makeCopy() = 0;

    void changePerm ( const std::string fileName, int perm ) { permissoin_.changePerm(perm); }
    void showInfo ( ) const {
        std::cout << permissoin_ << "  " << permissoin_.userName() << "  " << permissoin_.groupName() << "  ";
    }
    void open ( int mod ) {}
    void deleteFile ( ) {}

    virtual ~Descriptor ( ) {}

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
