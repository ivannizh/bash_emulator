#ifndef _DESCRIPTOR_H
#define _DESCRIPTOR_H

#include <iostream>

#include "../Date.h"
#include "Permission.h"
#include "../UserControl.h"

class Descriptor {
public:
    explicit Descriptor (const UserControl &uCtrl) : permissoin_(uCtrl) { }
    explicit Descriptor (const Permission& perm) : permissoin_(perm) {}

    Descriptor ( int userId, bool isDrive, const UserControl& uCtrl) : permissoin_(userId, isDrive, uCtrl) { }

    void showInfo() const;
    int  getOwner() const;
    int  getGroup() const;

    Permission& perm();

    virtual void deleteItSelf(int user) throw (Errors::PermissionDenied) = 0;
    virtual ~Descriptor() {}

protected:
    Date created_;
    Date changed_;
    Permission permissoin_;
//    std::string pointer_; // ?
};

#endif //_DESCRIPTOR_H
