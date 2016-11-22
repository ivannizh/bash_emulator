#ifndef _FILE_H
#define _FILE_H

#include "Descriptor.h"

class File: public Descriptor {
public: 
    File() {}

//    Descriptor* makeCopy() {}
    
    void makeReadable    ( ) {}
    void makeUnReadable  ( ) {}
    void makeWriteable   ( ) {}
    void makeUnWriteable ( ) {}
    void closeFile       ( ) {}

    void showInfo   ( ) const {}
    virtual void open       ( int mod ) {}

private: 
    unsigned int size_;
    bool isReadable_;
    bool isWriteable_;
    // храниние пользователя
};

#endif //_FILE_H
