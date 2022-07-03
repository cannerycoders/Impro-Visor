// From: https://rosettacode.org/wiki/S-expressions#C.2B.2B
#include "polya/Polylist.h"
#include <fstream>

int main(int argc, char** argv) 
{
    // std::ifstream is("../../vocab/My.voc", std::ios::binary); // no-comments, but large
    std::ifstream is("../../vocab/My.substitutions", std::ios::binary); // comments
    Polylist plist;
    bool dump(true);
    try 
    {
        plist.Parse(is, dump);
    } 
    catch (const std::exception& ex) 
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}