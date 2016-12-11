#include <iostream>
#include <Root.h>

using namespace std;

int main() {
//    std::string line = "\nroot\nmkdir d1 d2\ncd d1\nmkdir d1 d2 d3\n> d1/q d2/q d3/q q\ncd ..\ncp d1 d2\n";
//    for(size_t i = line.size()-1; i > 0; --i)
//        cin.putback(line[i]);
    Root().startWork();
    return 0;
}
