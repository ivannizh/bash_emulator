#ifndef _FILESTABLE_H
#define _FILESTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>

#include "Descriptor.h"

class FilesTable { // КОПИРУЙЮЩИЙ КОНСТРУКТОР
public: 
    typedef std::pair<std::string, Descriptor*> fileDescr;

    FilesTable(Descriptor* desc) { // const ??
        files_.push_back(fileDescr("." , desc));
        files_.push_back(fileDescr("..", desc));
    }

    FilesTable(const FilesTable &t) { throw std::runtime_error("Write func in FilesTable"); }
    FilesTable(const FilesTable &&t) { throw std::runtime_error("Write func in FilesTable"); }

    FilesTable& operator= (const FilesTable &)  { throw std::runtime_error("Write func in FilesTable"); }
    FilesTable& operator= (const FilesTable &&) { throw std::runtime_error("Write func in FilesTable"); }

    FilesTable& operator= (const FilesTable &)  const { throw std::runtime_error("Write func in FilesTable"); }
    FilesTable& operator= (const FilesTable &&) const { throw std::runtime_error("Write func in FilesTable"); }

    void addDescr    ( Descriptor *desc, const std::string &name) {
        files_.push_back(fileDescr(name, desc));
        return;
    }

    void showTable() const {
        for(const auto line: files_)
            std::cout << line.first << std::endl;
    }

    void deleteFile ( const std::string &name ) {}
    Descriptor* getFile(std::string name);


    void reName (const std::string &oldName, const std::string &newName);

    ~FilesTable() {}


private: 
    std::vector<fileDescr> files_;
};

#endif //_FILESTABLE_H
