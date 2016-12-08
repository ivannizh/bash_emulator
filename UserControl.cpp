#include "UserControl.h"

int Info::idCount = 1;

UserControl::UserControl() {

    users_.push_back(Info("root"));
    groups_.push_back(Info("root"));
    groups_[0] += users_[0].id();
    users_[0]  += groups_[0].id();
}

void UserControl::addUser(const std::__cxx11::string &name, const std::__cxx11::string &groupName){

    int grId = 0;

    if (groupName != "") {
        for(const Info &gr: groups_)
            if (gr.name() == groupName){
                grId = gr.id();
                break;
            }
        if (grId == 0) {
            groups_.push_back(Info(groupName));
            grId = groups_.back().id();
        }
    }

    for( Info &user: users_)
        if (user.name() == name) {
            if (grId){
                if (user().isIn(grId)) {
                    std::cout << "User '" << name << "' is already in group '" << groupName << "'" << std::endl;
                    return;
                } else {
                    for(Info &group: groups_)
                        if(group.name() == groupName) {
                            group += user.id();
                            user += group.id();
                            return;
                        }
                    groups_.push_back(groupName);
                    std::cout << "Group '" << groupName << "' was creared" << std::endl;
                    groups_.back() += user.id();
                    user += groups_.back().id();

                    return;
                }
            } else {
                std::cout << "User '" << name << "' is already exist '" << groupName << "'" << std::endl;
                return;
            }
        }

    users_.push_back(name);
    groups_.push_back(name);
    users_.back() += groups_.back().id();
    groups_.back() += users_.back().id();
    std::cout << "User '" << name << "' was created" << std::endl;

    if (groupName != ""){
        for(Info &group: groups_)
            if(group.name() == groupName) {
                group += users_.back().id();
                users_.back() += group.id();
                return;
            }
        groups_.push_back(groupName);
        std::cout << "Group '" << groupName << "' was created" << std::endl;
        groups_.back() += users_.back().id();
        users_.back() += groups_.back().id();

    }

    return;
}

bool UserControl::isUserInGroup(int user, int group) const {
    for (const Info& gr: groups_)
        if(gr.id() == group)
            return gr().isIn(user);
}

void UserControl::show(const std::vector<Info> &vec1, const std::vector<Info> &vec2, bool showGroups) const{
    if (showGroups){
        for (const Info &user: vec1) {
            std::cout << std::setw(10) << std::setfill(' ') << user.name()  << ": ";
            for (const int group: user().getIds())
                std::cout << getNameById(group, vec2) << " ";
            std::cout << std::endl;
        }
    } else {
        for (const Info &user: vec1)
            std::cout << user.name() << " ";
        std::cout << std::endl;
    }
}

void UserControl::showUsers(bool showGroups) const{
    show(users_, groups_, showGroups);
}

void UserControl::showGroups(bool showUsers) const{
        show(groups_, users_, showUsers);
}

bool UserControl::getAns(const std::string &ques){
    std::cout << ques << "[y/n]: ";
    char ans;
    std::cin >> ans;
    return (ans == 'y') ? true : false;
}

void UserControl::deleteId(std::vector<Info> &vec1, std::vector<Info> &vec2,
              const std::__cxx11::string &name, const std::string &type){
    if (name == "root"){
        std::cout << "Sorry u can't delete " << type << " 'root'" << std::endl;
        return;
    }
    for(auto i = vec1.begin(); i != vec1.end(); i++){
        if(i->name() == name){
            if (getAns("Are u sure u want delete " + type + " '" + i->name() + "'")) {
                for(Info &group: vec2)
                    group -= i->id();
                vec1.erase(i);
                return;
            }
            else
                return;
        }
    }
    std::cout << type << " '" << name << "' doesn't exist" << std::endl;
}

void UserControl::deleteUser(const std::__cxx11::string &name) {
    deleteId(users_, groups_, name, "user");
}

void UserControl::deleteGroup(const std::__cxx11::string &name) {
    deleteId(groups_, users_, name, "group");
}

std::string UserControl::getNameById(int id, const std::vector<Info> &vec) const {
    for(const Info &group: vec)
        if(group.id() == id)
            return group.name();
    return "";
}

std::__cxx11::string UserControl::getGroupNameById(int id) const {
    return getNameById(id, groups_);
}

std::__cxx11::string UserControl::getUserNameById(int id) const {
    return getNameById(id, users_);
}

int UserControl::getIdByName(const std::__cxx11::string &name, const std::vector<Info> &vec) const {
    for(const Info &user: vec)
        if(user.name() == name)
            return user.id();
    return 0;
}

int UserControl::getUserIdByName(const std::__cxx11::string &name) const{
    return getIdByName(name, users_);
}

int UserControl::getGroupIdByName(const std::__cxx11::string &name) const{
    return getIdByName(name, groups_);
}

std::string Info::name() const {
    return name_;
}

int Info::id() const {
    return id_;
}
