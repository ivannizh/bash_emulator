#ifndef _CATALOG_H
#define _CATALOG_H

#include <iostream>
#include <vector>

#include "Descriptor.h"
#include "FilesTable.h"
#include "File.h"

class Catalog: public Descriptor {
public: 
    Catalog() : fTable_(this) {}

    void creatFile ( const std::string &name ) {
        fTable_.addDescr(new File(), name);
    }
    void creatCatalog ( const std::string &name ) {
        fTable_.addDescr(new Catalog(), name);
    }
    
//    Descriptor* makeCopy() {}
    void showCatalog  ( ) const {
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
