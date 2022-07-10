#include "Leadsheet.h"
#include "polya/Polylist.h"
#include "util/Preferences.h"
#include "Score.h"
#include "Key.h"
#include "MelodyPart.h"
#include "ChordPart.h"

namespace Leadsheet
{

void saveLeadSheet(Score &score, bool saveRoadMap, std::ostream &ostr)
{
}

/**
 * Read leadsheet from tokens provided by Tokenizer into Score
 * @param in
 * @param score
 * @return 
*/
bool readLeadSheet(PlistTokenizer &t, Score &s)
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
bool readLeadSheet(PlistTokenizer &, Score &, 
                bool overrideStaveType, 
                Constants::StaveType useStaveType)
{
    return false;
}

std::string concatElements(Polylist &)
{
    return std::string();
}

bool extractChordsAndMelody(Polylist &chordsAndMelody,
                    Polylist &result)
{
    return false;
}

void addToMelodyPart(Polylist &melodyInputReversed,
                    MelodyPart &melody, 
                    int rise, int beatValue, 
                    Key &key)
{

}

bool populatePartWithChords(ChordPart &chordProg,
                    Polylist &chords,
                    int slotsAvailable,
                    int slotsPerBar)
{
    return false;
}

void addToChordPart(Polylist &chordInputReversed, 
                    ChordPart &chords,
                    int rise, int slotsPerBar, 
                    Key &key)
{

}

void addToMelodyFromPolylist(Polylist &in, 
                    MelodyPart &melody,
                    int rise, int slotsPerBeat,
                    Key &key)
{

}

int lookup(std::string &arg, std::vector<std::string> &table)
{
    return -1;
}

void classifyNotes(MelodyPart& melody, ChordPart& chords)
{
}

}; // end namespace