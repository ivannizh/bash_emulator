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

    FilesTable ( Descriptor* desc, Descriptor* parent ) {
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

    void deleteTable(int user) throw (Permission::PermissionDenied) {
        for(size_t i = files_.size() - 1; i > 0; --i){
            if(files_[i].first == "." || files_[i].first == ".."){
//                delete files_[i].second;
//                files_.erase(files_.begin() + i);
                continue;
            }
            try {
                files_[i].second->deleteItSelf(user);
//                delete files_[i].second;
//                files_.erase(files_.begin() + i);
            } catch (const Permission::PermissionDenied&) {
                throw;
            }
        }
    }

    void showTable ( ) const {
        for(const auto line: files_){
            line.second->showInfo();
            std::cout << line.first;
            std::cout << std::endl;
        }
    }

    void deleteFile ( const std::string &name ) {
        for(size_t i = 0; i < files_.size(); ++i)
            if(files_[i].first == name){
                delete files_[i].second;
                files_.erase(files_.begin() + i);
                return;
            }
    }

    Descriptor* getFile ( const std::string &name ) const {
        for(const auto &file: files_)
            if(file.first == name)
                return file.second;
        return files_[0].second;
    }
    Descriptor* getParent ( ) const {
        return files_[1].second;
    }

    std::string getName(const Descriptor* descr){
        for (const auto file: files_)
            if (file.second == descr)
                return file.first;
        return "";
    }

    int getOwner(const std::string& fName){
        for(const auto& file: files_)
            if (file.first == fName)
                return file.second->getOwner();
    }

    int getGroup(const std::string& fName){
        for(const auto& file: files_)
            if (file.first == fName)
                return file.second->getGroup();
    }

    void reName ( const std::string &oldName, const std::string &newName );

    ~FilesTable ( ) {
        for(fileDescr& file: files_){
            if(file.second != nullptr)
                delete file.second;
        }
    }


private:
    std::vector<fileDescr> files_;
};

#endif //_FILESTABLE_H
