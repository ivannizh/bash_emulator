#ifndef _CATALOG_H
#define _CATALOG_H

#include <iostream>
#include <vector>
#include <typeinfo>

#include "Descriptor.h"
#include "FilesTable.h"
#include "File.h"
#include "UserControl.h"

class Catalog: public Descriptor {
public: 
    Catalog ( const UserControl& uCtrl ) : Descriptor(uCtrl), fTable_(this, this) { }
    Catalog ( const Permission& perm, Catalog* parent) : Descriptor(perm), fTable_(this, parent) {}
    Catalog ( int userId, Catalog* parent, const UserControl& uCtrl ) : Descriptor(userId, true, uCtrl), fTable_(this, parent) {}

    std::string getDirName(const Catalog* cat);

    void creatFile ( const std::string &name, int userId );
    void creatCatalog ( const std::string &name, int userId );

    void showCatalog (std::string dir, int user) const throw(std::invalid_argument);
    void deleteFile (const std::string& file, bool isRec, const int user) throw (Errors::PermissionDenied);

    bool checkX(int user) const;
    bool checkW(int user) const;
    bool checkR(int user) const;

    void delRef(const std::string& fName);
    void addRef(const std::string& fName, Descriptor* descr);

    friend std::ostream& operator<< (std::ostream& os, const Catalog& cat);

    Catalog* getCatalog(const std::string &name);

    File* getFile(const std::string &name);

    Descriptor* getDescr(const std::string &name);

    int getOwner(const std::string& fName);
    int getGroup(const std::string& fName);

    void deleteItSelf(int user) throw (Errors::PermissionDenied);

    void chowner(const std::string& fName, const std::string& newUser, const std::string& newGroup, int user) throw (Errors::PermissionDenied);

    Descriptor* retCopy(const std::string& fName, Catalog* parent);

    Catalog* getCopy(Catalog* parent);

    virtual ~Catalog(){}
private:
    FilesTable fTable_;

    bool checkParentWritePerm(int user);

};

#endif //_CATALOG_H
