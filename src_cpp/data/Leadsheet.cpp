#include "Leadsheet.h"
#include "polya/Polylist.h"
#include "util/Preferences.h"

namespace Leadsheet
{

/**
 * Read leadsheet from tokens provided by Tokenizer into Score
 * @param in
 * @param score
 * @return 
*/
bool readLeadSheet(class Tokenizer &t, class Score &s)
{
    return readLeadSheet(t, s, 
            Preferences::get().getAlwaysUseStave(),
            Preferences::get().getStaveType());
}

/**
 * Read leadsheet from tokens provided by Tokenizer into Score, with
 * a given StaveType, which can be overridden.
 * @param in
 * @param score
 * @return 
 */
bool readLeadSheet(class Tokenizer &, 
                    class Score &, 
                    bool overrideStaveType, 
                    Constants::StaveType useStaveType)
{
    return false;
}


void saveLeadSheet(class Score &, bool saveRoadMap, std::ostream &)
{
}

std::string concatElements(class Polylist &)
{
    return std::string();
}

bool extractChordsAndMelody(class Polylist &chordsAndMelody,
                    class Polylist &result)
{
    return false;
}

void addToMelodyPart(class Polylist &melodyInputReversed,
                    class MelodyPart &melody, 
                    int rise, int beatValue, 
                    class Key &key)
{

}

bool populatePartWithChords(class ChordPart &chordProg,
                    class Polylist &chords,
                    int slotsAvailable,
                    int slotsPerBar)
{
    return false;
}

void addToChordPart(class Polylist &chordInputReversed, 
                    class ChordPart &chords,
                    int rise, int slotsPerBar, 
                    class Key &key)
{

}

void addToMelodyFromPolylist(class Polylist &in, 
                    class MelodyPart &melody,
                    int rise, int slotsPerBeat,
                    class Key &key)
{

}

int lookup(std::string &arg, std::vector<std::string> &table)
{
    return -1;
}

void classifyNotes(class MelodyPart& melody, class ChordPart& chords)
{
}

}; // end namespace