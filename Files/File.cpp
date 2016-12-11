#include "File.h"

File *File::getCopy() const{
    return new File(*this);
}
