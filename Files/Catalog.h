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

//    Catalog& operator= (const Catalog& c){
//        fTable_ = c.fTable_;
//    }
//    Catalog& operator= (Catalog&& c){
//        if(&c != this)
//            fTable_ = c.fTable_;
//    }

    Catalog* getCatalog(const std::string &name) throw (std::bad_cast){

        try {
            Catalog* cat = dynamic_cast<Catalog*>(fTable_.getFile(name));
            return cat;
        }
        catch (const std::bad_cast& r) {
            int a = 5;
            return dynamic_cast<Catalog*>(fTable_.getCurDir());
        }
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
