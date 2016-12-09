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
    Catalog ( int userId, const UserControl& uCtrl ) : fTable_(this, this), Descriptor(uCtrl) { }
    Catalog ( int userId, Catalog* parent, const UserControl& uCtrl ) : fTable_(this, parent), Descriptor(userId, true, uCtrl) {}

    void creatFile ( const std::string &name, int userId );
    void creatCatalog ( const std::string &name, int userId );

    std::string getDirName(const Catalog* cat);

    void showCatalog (std::string dir, int user) const throw(std::invalid_argument){
        if(dir == ""){
            fTable_.showTable();
            return;
        }
        int npos = dir.find("/");
        std::string nextDir = dir.substr(0, npos);
        if(npos < 0)
            dir.clear();
        else
            dir.erase(0, npos+1);

        Catalog* cat = dynamic_cast<Catalog*>(fTable_.getFile(nextDir));
        if(cat == this)
           throw std::invalid_argument("");

        if (cat->checkR(user)){
            try {
                cat->showCatalog(dir, user);
            } catch (const std::invalid_argument&) {
                throw;
            }
        } else {
            std::cout  << "\033[31m" << "Permission denied : " << "\033[0m"  << nextDir << std::endl;
        }
    }

    void deleteFile (const std::string& file, bool isRec, const int user) throw (Permission::PermissionDenied){

        if(fTable_.getFile(file) == this) {
            std::cout << "No such file or directory : " << file << std::endl;
            return;
        }

        if(perm().checkW(user)) {
            Descriptor* descr = fTable_.getFile(file);
            if (descr->perm().isDir()){
                if(!isRec){
                    std::cout << "cannot remove '" << file << "': Is a directory" << std::endl;
                    return;
                }
                try {
                    dynamic_cast<Catalog*>(descr)->fTable_.deleteTable(user);
                    fTable_.deleteFile(file);
                } catch(const Permission::PermissionDenied&) {
                    throw;
                }
            } else {
                fTable_.deleteFile(file);
            }
        } else
            throw Permission::PermissionDenied();
    }

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

    void deleteItSelf(int user) throw (Permission::PermissionDenied){
        if (!perm().checkX(user) || !perm().checkW(user))
            throw Permission::PermissionDenied();
        fTable_.deleteTable(user);
    }

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

    bool checkParentWritePerm(int user){
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
    }
};

#endif //_CATALOG_H
