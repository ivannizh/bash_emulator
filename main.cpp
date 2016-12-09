#include <iostream>
#include <Root.h>

using namespace std;

int main() {
//    std::string line = "\nroot\nmkdir d1\naddu ivan\nchowner d1 -u ivan\nexit\nivan\nrm d1 -r\n";
//    for(size_t i = line.size()-1; i > 0; --i)
//        cin.putback(line[i]);
    Root().startWork();
    return 0;
}
