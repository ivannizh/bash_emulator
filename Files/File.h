#ifndef _FILE_H
#define _FILE_H

#include "Descriptor.h"

class File: public Descriptor {
public: 
    File(int userId, const UserControl& uCrtl) : Descriptor(userId, false, uCrtl),
      isReadable_(true), isWriteable_(true), userId_(0){}

    File* getCopy() const;
    
    void makeReadable    (int user);
    void makeUnReadable  (int user);
    void makeWriteable   (int user);
    void makeUnWriteable (int user);

    void write (const std::string& line, int user){
        if(!isReadable_ || !isWriteable_)
            if(user != userId_){
                std::cout << "Sorry, file is blocked" << std::endl;
                return;
            }
        fInfo += line;
    }

    void cat (int user) const{
        if(!isWriteable_)
            if(user != userId_){
                std::cout << "Sorry, file is blocked" << std::endl;
                return;
            }
        std::cout << fInfo << std::endl;
    }

    void clean(int user){
        if(!isReadable_ || !isWriteable_)
            if(user != userId_){
                std::cout << "Sorry, file is blocked" << std::endl;
                return;
            }
        fInfo.clear();
    }

    void deleteItSelf(int) throw (Errors::PermissionDenied){}

    ~File(){}

private: 
    std::string fInfo;
    bool isReadable_;
    bool isWriteable_;
    int userId_;
};

#endif //_FILE_H
