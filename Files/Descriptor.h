#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "Permission.h"
#include "../UserControl.h"

class Descriptor {
public: 

    Descriptor(int userId) : userId_(userId) {}

//    virtual Descriptor* makeCopy() = 0;

    void addPer     ( int perm ) {}
    void remPer     ( int perm ) {}
    void showInfo   ( const UserControl &uControl ) const {
        std::cout << userId_ << std::endl;
    }
    void open ( int mod ) {}
    void deleteFile ( ) {}

//    enum descrType {FILE, CATALOG};

    virtual ~Descriptor() {}

protected: 
    int userId_;
    int groupId_;
    Date created_;
    Date changed_;
    Permission permissoin_;
    std::string pointer_; // ?
};

#endif //_DESCRIPTOR_H
