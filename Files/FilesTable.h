#ifndef _FILESTABLE_H
#define _FILESTABLE_H

#include <iostream>
#include <vector>

#include "Descriptor.h"

class FilesTable { // КОПИРУЙЮЩИЙ КОНСТРУКТОР
public: 
    FilesTable() {}

    FilesTable(const FilesTable &t) {}

    void addFile    ( const Descriptor &desc  ) {}
    void deleteFile ( const std::string &name ) {}
    Descriptor* getFile(std::string name);


    void reName (const std::string &oldName, const std::string &newName);

    ~FilesTable() {}


private: 
    std::vector<std::pair<std::string, Descriptor*>> files_;
    // продумать создание указателя на этот каталог и на родителя
};

#endif //_FILESTABLE_H
