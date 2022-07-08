#pragma once

#include <string>

/**
 * MelodySYmbol represents symbols that can occur in a melody.
 * Currently these are NoteSymbol and VolumeSymbol.
 */
class MelodySymbol
{
public:
    /**
     * Make a melody symbol from a String representing either a note, rest, or
    * volume.
    * @param string
    * @return 
    */
    static std::shared_ptr<MelodySymbol> makeMelodySymbol(std::string const &str);
};