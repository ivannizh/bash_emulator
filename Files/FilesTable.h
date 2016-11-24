#ifndef _FILESTABLE_H
#define _FILESTABLE_H

#include <iostream>
#include <vector>
#include <stdexcept>
#include <iomanip>

#include "Descriptor.h"
#include "../UserControl.h"

class FilesTable {
public: 
    typedef std::pair<std::string, Descriptor*> fileDescr;

    FilesTable(Descriptor* desc) { // const ??
        files_.push_back(fileDescr("." , desc));
        files_.push_back(fileDescr("..", desc));
    }

    FilesTable(const FilesTable &t) { throw std::runtime_error("Write func1 in FilesTable"); }
    FilesTable(const FilesTable &&t) { throw std::runtime_error("Write func2 in FilesTable"); }

    FilesTable& operator= (const FilesTable &)  { throw std::runtime_error("Write func3 in FilesTable"); }
    FilesTable& operator= (const FilesTable &&) { throw std::runtime_error("Write func4 in FilesTable"); }
    void addDescr    ( Descriptor *desc, const std::string &name) {
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

    void showTable() const {
        for(const auto line: files_){
            std::cout << std::setw(10) << line.first << "  ";
            line.second->showInfo();
            std::cout << std::endl;
        }
    }

    void deleteFile ( const std::string &name ) {}
    Descriptor* getFile(std::string name);


    void reName (const std::string &oldName, const std::string &newName);

    ~FilesTable() {}


private:
    std::vector<fileDescr> files_;
};

#endif //_FILESTABLE_H
