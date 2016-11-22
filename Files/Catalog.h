#ifndef _CATALOG_H
#define _CATALOG_H

#include <iostream>
#include <vector>

#include "Descriptor.h"
#include "FilesTable.h"

class Catalog: public Descriptor {
public: 
    Catalog();
    
    void creatFile    ( ) {}
    void showCatalog  ( ) const {}
    void creatCatalog ( ) {}
    void copy         ( const std::string &fileName, const std::string &dist )    {}
    void reName       ( const std::string &fileName, const std::string &newName ) {}
    void deleteFile   ( const std::string &fileName )   {}
    void goNextDir    ( const std::string &name ) const {}
//    void reName       ( const std::string &oldName, const std::string &newName  ) {}

    virtual void open       ( const std::string &name, int mod ) {}
    void showInfo ( ) const {}
};

#endif //_CATALOG_H
