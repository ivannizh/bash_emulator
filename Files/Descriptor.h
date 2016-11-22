#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "../User.h"
#include "Permission.h"

class Descriptor {
public: 

    Descriptor() {}

//    virtual Descriptor* makeCopy() = 0;

    void addPer     ( int perm ) {}
    void remPer     ( int perm ) {}
    void showInfo   ( ) const;
    void open ( int mod ) {}
    void deleteFile ( ) {}
    virtual ~Descriptor() {}

protected: 
    User user_;
    UserGroup group_;
    Date created_;
    Date changed_;
    Permission permissoin_;
    std::string pointer_; // ?
};

#endif //_DESCRIPTOR_H
