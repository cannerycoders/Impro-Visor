#include "polya/Polylist.h"
#include "data/advice/Advisor.h"
#include <fstream>

int main(int argc, char** argv) 
{
    // std::ifstream is("../../vocab/My.voc", std::ios::binary); // no-comments, but large
    std::ifstream is("../../vocab/My.voc", std::ios::binary); // comments
    if(!is.is_open())
        is.open("../vocab/My.voc", std::ios::binary);
    Polylist::Ptr plist = Polylist::MakePtr();
    Advisor adv;
    bool dump(false);
    try 
    {
        plist->Parse(is, dump);
        adv.SetRules(plist);
    } 
    catch (const std::exception& ex) 
    {
        std::cerr << ex.what() << '\n';
    }
    return 0;
}
