#include "polya/Polylist.h"
#include "data/advice/Advisor.h"
#include <fstream>

int main(int argc, char** argv) 
{
    // std::ifstream is("../../vocab/My.voc", std::ios::binary); // no-comments, but large
    std::ifstream is("../../vocab/My.voc", std::ios::binary); // comments
    Polylist plist;
    Advisor adv;
    bool dump(false);
    try 
    {
        adv.SetRules(plist.Parse(is, dump));
    } 
    catch (const std::exception& ex) 
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}