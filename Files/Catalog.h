#ifndef _CATALOG_H
#define _CATALOG_H

#include <iostream>
#include <vector>

#include "Descriptor.h"
#include "FilesTable.h"
#include "File.h"
#include "UserControl.h"

class Catalog: public Descriptor {
public: 
    Catalog ( int userId, const UserControl& uCtrl ) : fTable_(this, this), Descriptor(userId, true, uCtrl) { }
    Catalog ( int userId, Catalog* parent, const UserControl& uCtrl ) : fTable_(this, parent), Descriptor(userId, true, uCtrl) {}

    void creatFile ( const std::string &name, int userId ) {
        fTable_.addDescr(new File(userId, uControl_), name);
    }
    void creatCatalog ( const std::string &name, int userId ) {
        fTable_.addDescr(new Catalog(userId, this, uControl_), name);
    }

    void showCatalog () const {
        fTable_.showTable();
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


};

#endif //_CATALOG_H
