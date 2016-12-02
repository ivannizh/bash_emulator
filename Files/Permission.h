#ifndef _PERMISSION_H
#define _PERMISSION_H

#include <vector>
#include <iostream>
//
// d rwx
// 1 421

class Permission {
public: 
    Permission ( bool isDrive ) : d_(isDrive) {
        if (isDrive){
            p_.push_back(7);
            p_.push_back(7);
            p_.push_back(5);
        } else {
            p_.push_back(6);
            p_.push_back(6);
            p_.push_back(4);
        }
    }
    
    void changePerm ( int perm ) {
       if(perm < 100 || perm > 777){
           std::cout << "Wrong permision value" << std::endl;
           return;
       }

       for(char it: p_){
           it = perm%10;
           perm /= 10;
       }
    }

    friend std::ostream& operator<< ( std::ostream& os, const Permission &perm ){
        perm.d_ ? os<<'d' : os<<'-';
        for(char it: perm.p_){
            it & 4 ? os<<'r' : os<<'-';
            it & 2 ? os<<'w' : os<<'-';
            it & 1 ? os<<'x' : os<<'-';
        }
        return os;
    }

private:
    bool d_;
    std::vector<char> p_;
};

#endif //_PERMISSION_H
