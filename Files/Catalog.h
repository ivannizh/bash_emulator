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
    Catalog ( int userId, Catalog* parent, const UserControl& uCtrl ) : Descriptor(userId, true, uCtrl), fTable_(this, parent) {}

    void creatFile ( const std::string &name, int userId );
    void creatCatalog ( const std::string &name, int userId );

    std::string getDirName(const Catalog* cat);

    void showCatalog (std::string dir, int user) const throw(std::invalid_argument);

    void deleteFile (const std::string& file, bool isRec, const int user) throw (Errors::PermissionDenied);

    bool checkX(int user) const {
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkX(user);
    }
    bool checkW(int user) const {
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
    }
    bool checkR(int user) const {
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkR(user);
    }

    friend std::ostream& operator<< (std::ostream& os, const Catalog& cat);

    Catalog* getCatalog(const std::string &name) {
        try                          { return dynamic_cast<Catalog*>(getDescr(name)); }
        catch (const std::bad_cast&) { return dynamic_cast<Catalog*>(fTable_.getCurDir());   }
    }

    File* getFile(const std::string &name) {
        try                          { return dynamic_cast<File*>(getDescr(name)); }
        catch (const std::bad_cast&) { return dynamic_cast<File*>(fTable_.getCurDir());   }
    }

    Descriptor* getDescr(const std::string &name) {
        return fTable_.getFile(name);
    }

    int getOwner(const std::string& fName) {
        return fTable_.getOwner(fName);
    }

    int getGroup(const std::string& fName) {
        return fTable_.getGroup(fName);
    }

    void deleteItSelf(int user) throw (Errors::PermissionDenied){
        if (!perm().checkX(user) || !perm().checkW(user))
            throw Errors::PermissionDenied();
        fTable_.deleteTable(user);
    }

    void chowner(const std::string& fName, const std::string& newUser, const std::string& newGroup, int user) throw (Errors::PermissionDenied){
        int userId  = permissoin_.uControl().getUserIdByName  ( newUser  );
        int groupId = permissoin_.uControl().getGroupIdByName ( newGroup );

        if(userId == 0 && groupId == 0) {
            std::cout << "Wrong user or group name" << std::endl;
            return;
        }

        Descriptor* file = fTable_.getFile(fName);

        if (file == this){
            Errors::noFile(fName);
        }

        if(permissoin_.uControl().isUserInGroup(user, 2) ||
                permissoin_.uControl().isUserInGroup(user, file->perm().groupId()) ||
                file->getOwner() == user){
            if (userId  != 0)
                file->perm().newUser  ( userId  );
            if (groupId != 0)
                file->perm().newGroup ( groupId );

        } else {
            Errors::PermissionDenied::printError();
            return;
        }
    }
    ~Catalog(){}

//    void copy         ( const std::string &fileName, const std::string &dist )    {}
//    void reName       ( const std::string &fileName, const std::string &newName ) {}
//    void deleteFile   ( const std::string &fileName )   {}
//    void goNextDir    ( const std::string &name ) const {}
//    void reName       ( const std::string &oldName, const std::string &newName  ) {}

//    void open       ( const std::string &name, int mod ) {}
//    void showInfo ( ) const {}
//    virtual ~Catalog(){}
private:
    FilesTable fTable_;

    bool checkParentWritePerm(int user);

};

#endif //_CATALOG_H
