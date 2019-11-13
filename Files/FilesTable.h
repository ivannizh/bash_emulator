#ifndef _FILESTABLE_H
#define _FILESTABLE_H

#include <iostream>
#include <stdexcept>
#include <iomanip>

#include "myvector.h"
#include "Descriptor.h"
#include "../UserControl.h"

class FilesTable {
public: 
    typedef std::pair<std::string, Descriptor*> fileDescr;

    FilesTable ( Descriptor* desc, Descriptor* parent );

    Descriptor* getCurDir() const;

    void delRef(const std::string& fName);

    void addRef(const std::string& fName, Descriptor* descr);

    FilesTable ( const FilesTable &  ) { throw std::runtime_error("Write func1 in FilesTable"); }
    FilesTable ( const FilesTable && ) { throw std::runtime_error("Write func2 in FilesTable"); }

    FilesTable& operator= ( const FilesTable & ) { throw std::runtime_error("Write func3 in FilesTable"); }
    FilesTable& operator= ( const FilesTable && ) { throw std::runtime_error("Write func4 in FilesTable"); }

    void addDescr ( Descriptor *desc, const std::string &name );

    void deleteTable(int user) throw (Errors::PermissionDenied);

    void showTable ( ) const;

    void deleteFile ( const std::string &name );

    Descriptor* getFile ( const std::string &name ) const;
    Descriptor* getParent ( ) const;

    std::string getName(const Descriptor* descr);

    int getOwner(const std::string& fName);

    int getGroup(const std::string& fName);

    void reName ( const std::string &oldName, const std::string &newName );

    ~FilesTable ( );

    const MyVector<fileDescr>& getFiles() const;

private:
    MyVector<fileDescr> files_;
};

#endif //_FILESTABLE_H
