#ifndef _FILESTABLE_H
#define _FILESTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include "Descriptor.h"
//#include "Catalog.h"
#include "../UserControl.h"

class FilesTable {
public: 
    typedef std::pair<std::string, Descriptor*> fileDescr;

    FilesTable ( Descriptor* desc, Descriptor* parent ) { // const ??
        files_.push_back(fileDescr("." , desc));
        files_.push_back(fileDescr("..", parent));
    }

    Descriptor* getCurDir() const {
        return files_[0].second;
    }

    FilesTable ( const FilesTable & )  { throw std::runtime_error("Write func1 in FilesTable"); }
    FilesTable ( const FilesTable && ) { throw std::runtime_error("Write func2 in FilesTable"); }

    FilesTable& operator= ( const FilesTable & ) { throw std::runtime_error("Write func3 in FilesTable"); }
    FilesTable& operator= ( const FilesTable && ) { throw std::runtime_error("Write func4 in FilesTable"); }

    void addDescr ( Descriptor *desc, const std::string &name ) {
        std::string tmp = name;
        if(tmp.back() == '/')
            tmp.erase(name.length()-1, 1);

        for (const auto file:files_)
            if(file.first == tmp){
                std::cout << "cannot create file/directory ‘" << tmp << "’: File exists" << std::endl;
                return;
            }

        files_.push_back(fileDescr(tmp, desc));
        return;
    }

    void showTable ( ) const {
        for(const auto line: files_){
            line.second->showInfo();
            std::cout << line.first;
            std::cout << std::endl;
        }
    }

    void deleteFile ( const std::string &name ) {}

    Descriptor* getFile ( const std::string &name ) const {
        for(const auto &file: files_)
            if(file.first == name)
                return file.second;
        return files_[0].second;
    }

    std::string getName(const Descriptor* descr){
        for (const auto file: files_)
            if (file.second == descr)
                return file.first;
        return "";
    }

    void reName ( const std::string &oldName, const std::string &newName );

    ~FilesTable ( ) {}


private:
    std::vector<fileDescr> files_;
};

#endif //_FILESTABLE_H
