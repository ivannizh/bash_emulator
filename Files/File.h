#ifndef _FILE_H
#define _FILE_H

#include "Descriptor.h"

class File: public Descriptor {
public: 
    File(int userId, const UserControl& uCrtl) : Descriptor(userId, false, uCrtl) {}

    File* getCopy() const;
    
    void makeReadable    ( ) {} // TODO
    void makeUnReadable  ( ) {} // TODO
    void makeWriteable   ( ) {} // TODO
    void makeUnWriteable ( ) {} // TODO
    void closeFile       ( ) {} // TODO

    void deleteItSelf(int) throw (Errors::PermissionDenied){}

    ~File(){}

//    void showInfo   ( ) const {}
//    virtual void open       ( int mod ) {}
//    virtual ~File(){}
private: 
//    unsigned int size_;
//    bool isReadable_;
//    bool isWriteable_;
    // храниние пользователя
};

#endif //_FILE_H
