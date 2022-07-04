#include "data/PitchClass.h"
#include <iostream>
#include <vector>

void dumpPC(PitchClass const *pc)
{
    std::cerr << "PitchClass " << pc->toString()
        << ": " << pc->getIndex() 
        << " " << pc->getChordBase() << "\n";
}

int main(int argc, char** argv) 
{
    PitchClass const *cp = &PitchClass::getPitchClass(8);
    std::cerr << "Circle of fifths ascending\n";
    for(int i=0;i<12;i++)
    {
        dumpPC(cp);
        cp = &(cp->transpose(7));
    }
    return 0;
}