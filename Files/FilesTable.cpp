#include "FilesTable.h"

FilesTable::FilesTable(Descriptor *desc, Descriptor *parent) {
    files_.push_back(fileDescr("." , desc));
    files_.push_back(fileDescr("..", parent));
}

Descriptor *FilesTable::getCurDir() const {
    return files_[0].second;
}

void FilesTable::delRef(const std::__cxx11::string &fName){
    for(auto i = files_.begin(); i != files_.end(); ++i)
        if(i->first == fName){
            files_.erase(i);
            return;
        }
}

void FilesTable::addRef(const std::__cxx11::string &fName, Descriptor *descr){
    for(fileDescr file: files_)
        if(file.first == fName){
            delete file.second;
            file.second = descr;
            return;
        }
    files_.push_back(fileDescr(fName, descr));
}

void FilesTable::addDescr(Descriptor *desc, const std::__cxx11::string &name) {
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

void FilesTable::deleteTable(int user) throw (Errors::PermissionDenied){
    for(size_t i = files_.size() - 1; i > 0; --i){
        if(files_[i].first == "." || files_[i].first == ".."){
            continue;
        }
        try {
            files_[i].second->deleteItSelf(user);
        } catch (const Errors::PermissionDenied&) {
            throw;
        }
    }
}

void FilesTable::showTable() const {
    for(const fileDescr line: files_){
        line.second->showInfo();
        std::cout << line.first;
        std::cout << std::endl;
    }
}

void FilesTable::deleteFile(const std::__cxx11::string &name) {
    for(size_t i = 0; i < files_.size(); ++i)
        if(files_[i].first == name){
            delete files_[i].second;
            files_.erase(files_.begin() + i);
            return;
        }
}

Descriptor *FilesTable::getFile(const std::__cxx11::string &name) const {
    for(const auto &file: files_)
        if(file.first == name)
            return file.second;
    return files_[0].second;
}

Descriptor *FilesTable::getParent() const {
    return files_[1].second;
}

std::__cxx11::string FilesTable::getName(const Descriptor *descr){
    for (const auto file: files_)
        if (file.second == descr)
            return file.first;
    return "";
}

int FilesTable::getOwner(const std::__cxx11::string &fName){
    for(const auto& file: files_)
        if (file.first == fName)
            return file.second->getOwner();
    return 0;
}

int FilesTable::getGroup(const std::__cxx11::string &fName){
    for(const auto& file: files_)
        if (file.first == fName)
            return file.second->getGroup();
    return 0;
}

FilesTable::~FilesTable() {
    files_[0].second = nullptr;
    files_[1].second = nullptr;
    for(fileDescr& file: files_){
        if(file.second != nullptr)
            delete file.second;
    }
}

const std::vector<FilesTable::fileDescr>& FilesTable::getFiles() const
{
    return files_;
}
