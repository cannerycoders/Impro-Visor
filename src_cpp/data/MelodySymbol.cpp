#include "MelodySymbol.h"

#include "NoteSymbol.h"
#include "VolumeSymbol.h"

/*static*/std::shared_ptr<MelodySymbol>
MelodySymbol::makeMelodySymbol(std::string const &str)
{
    char firstChar = str[0];
    switch(firstChar)
    {
    case 'a':
    case 'b':
    case 'c':
    case 'd':
    case 'e':
    case 'f':
    case 'g':
    case 'r':
        {
            NoteSymbol::NSPtr x = NoteSymbol::makeNoteSymbol(str);
            return std::static_pointer_cast<MelodySymbol>(x);
        }
        break;
    case 'v':
        {
            VolumeSymbol::VSPtr x = VolumeSymbol::makeVolumeSymbol(str);
            return std::static_pointer_cast<MelodySymbol>(x);
        }
    default:
        break;
    }
    return std::shared_ptr<MelodySymbol>();
}