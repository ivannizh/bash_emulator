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

    void showCatalog () const {
        fTable_.showTable();
    }

    bool checkX(int user) const {
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkX(user);
    }

    friend std::ostream& operator<< (std::ostream& os, const Catalog& cat);

    Catalog* getCatalog(const std::string &name) {
        try                          { return dynamic_cast<Catalog*>(fTable_.getFile(name)); }
        catch (const std::bad_cast&) { return dynamic_cast<Catalog*>(fTable_.getCurDir());   }
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
        return dynamic_cast<Catalog*>(fTable_.getCurDir())->permissoin_.checkWrite(user);
    }
};

#endif //_CATALOG_H
