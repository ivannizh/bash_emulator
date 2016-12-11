#ifndef USERCONTROL_H
#define USERCONTROL_H

#include <iostream>
#include <vector>
#include <iomanip>

class BaseUser {
public:
    BaseUser() {}

    void addId    ( int id );
    void deleteId ( int id );

    bool isIn(int checkId) const;
    const std::vector<int>& getIds() const;

protected:
    std::vector<int> ids_;
};

class Info {
public:
    Info (const std::string &name) : buser_(BaseUser()), name_(name), id_(idCount++) { }

    Info& operator+  ( int id ) { buser_.addId(id);    return *this; }
    Info& operator+= ( int id ) { buser_.addId(id);    return *this; }
    Info& operator-  ( int id ) { buser_.deleteId(id); return *this; }
    Info& operator-= ( int id ) { buser_.deleteId(id); return *this; }
    const BaseUser& operator ()( ) const { return buser_; }

    std::string name() const;
    int id() const;

private:
    static int idCount;
    BaseUser buser_;
    std::string name_;
    int id_;
};

class UserControl {
public:
    UserControl();

    void addUser(const std::string &name, const std::string &groupName);
    bool isUserInGroup (int user, int group) const;

    void showUsers   ( bool showGroups         ) const;
    void showGroups  ( bool showUsers          ) const;
    void deleteUser  ( const std::string &name );
    void deleteGroup ( const std::string &name );

    std::string getGroupNameById ( int id ) const;
    std::string getUserNameById  ( int id ) const;

    int getUserIdByName  ( const std::string &name          ) const;
    int getGroupIdByName ( const std::__cxx11::string &name ) const;

private:
    std::vector<Info> users_;
    std::vector<Info> groups_;

    void deleteId(std::vector<Info>&vec1, std::vector<Info>&vec2, const std::__cxx11::string&name, const std::string&type);
    void show(const std::vector<Info> &vec1, const std::vector<Info> &vec2, bool showGroups) const;
    std::string getNameById(int id, const std::vector<Info> &vec) const;
    int getIdByName(const std::__cxx11::string &name, const std::vector<Info> &vec) const;
    bool getAns(const std::string &ques);
};


#endif // USERCONTROL_H
