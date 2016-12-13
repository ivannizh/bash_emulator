#include "File.h"

File *File::getCopy() const{
    return new File(*this);
}

void File::makeReadable(int user) {
    if (user != userId_){
        std::cout << "Another user blocked file" << std::endl;
        return;
    }
    isReadable_ = true;
}

void File::makeUnReadable(int user) {
    userId_ = user;
    isReadable_ = false;
}

void File::makeWriteable(int user) {
    if (user != userId_){
        std::cout << "Another user blocked file" << std::endl;
        return;
    }
    isReadable_ = true;
}

void File::makeUnWriteable(int user) {
    userId_ = user;
    isWriteable_ = false;
}
