#include "Constants.h"
#include <iostream>

int main(int argc, char** argv) 
{
    using namespace Constants;
    std::cerr << "CROOT " << CROOT << "\n";
    std::cerr << "knownNoteValue\n"; 
    for(auto i : knownNoteValue)
        std::cerr << "  " << i << "\n";
    return 0;
}