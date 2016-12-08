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

    void showCatalog (std::string dir, int user) const throw(std::invalid_argument){ //TODO : think about const ref
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

        if (cat->checkR(user)){ // BUG
            try {
                cat->showCatalog(dir, user);
            } catch (const std::invalid_argument&) {
                throw;
            }
        } else {
            std::cout  << "\033[31m" << "Permission denied : " << "\033[0m"  << nextDir << std::endl;
        }
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


//    void copy         ( const std::string &fileName, const std::string &dist )    {}
//    void reName       ( const std::string &fileName, const std::string &newName ) {}
//    void deleteFile   ( const std::string &fileName )   {}
//    void goNextDir    ( const std::string &name ) const {}
//    void reName       ( const std::string &oldName, const std::string &newName  ) {}

//    void open       ( const std::string &name, int mod ) {}
//    void showInfo ( ) const {}
private:
    FilesTable fTable_;

    bool checkParentWritePerm(int user){
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkW(user);
    }
};

#endif //_CATALOG_H
