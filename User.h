#ifndef _USER_H
#define _USER_H

#include <iostream>
#include <vector>

class UserGroup;

class User {
public: 
    User() {}

    void addGroup ( const std::string &name )    {}
    void reName   ( const std::string &newName ) {}

private: 
    std::string name_;
    std::vector<UserGroup> groups_;
};

class UserGroup {
public:
    UserGroup();

    void addUser    ( const std::string &name )    {}
    void deleteUser ( const std::string &name )    {}
    void reName     ( const std::string &newName ) {}

private:
    std::string name_;
    std::vector<User> users_;
};

#endif //_USER_H
