#include "Permission.h"

int Permission::userId() const
{
    return userId_;
}

int Permission::groupId() const
{
    return groupId_;
}

const UserControl &Permission::uControl() const
{
    return uControl_;
}
