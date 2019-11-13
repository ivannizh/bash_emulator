#ifndef _PERMISSION_H
#define _PERMISSION_H

#include <iostream>

#include "myvector.h"
#include "../UserControl.h"

// d rwx
// 1 421

class Errors {
public:
    static void noFile(const std::string& file = "") {
        std::cout << "No such file or directory : " << file << std::endl;
    }
    static void noCatalog(const std::string& file = "") {
        std::cout << "No such    directory : " << file << std::endl;
    }
    static void missedFiles() {
        std::cout << "Missed files" << std::endl;
    }
    class PermissionDenied {
    public:
        PermissionDenied() {}
        static void printError() {
            std::cout << "\033[31m" << "Permission denied" << "\033[0m" << std::endl;
        }
    };
};

class Permission {
public:
    explicit Permission ( const UserControl& uCtrl);
    Permission ( int user, bool isDrive, const UserControl& uCtrl);

    bool isDir() const { return d_; }

    bool checkR (int user) { return checkPerm(4, user); }
    bool checkW (int user) { return checkPerm(2, user); }
    bool checkX (int user) { return checkPerm(1, user); }

    void changePerm ( int perm  );
    void newGroup   ( int group );
    void newUser    ( int user  );

    Permission& operator+= (const std::string& perm);
    Permission& operator-= (const std::string& perm);
    Permission& operator=  (const std::string& perm);
    

    friend std::ostream& operator<< ( std::ostream& os, const Permission &perm );

    int userId() const;
    int groupId() const;

    std::string userName() const;
    std::string groupName() const;

    const UserControl &uControl() const;

private:
    bool d_;
    const UserControl& uControl_;
    int userId_;
    int groupId_;
    MyVector<char> p_;

    void forOperators(const std::string& q, void (*func)(char&, const char&));
    bool checkPerm(int type, int user);
};

#endif //_PERMISSION_H
