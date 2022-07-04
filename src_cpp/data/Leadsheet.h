#pragma once

#include "Constants.h"
#include <ostream>

namespace Leadsheet
{
    enum Component
    {
        TITLE = 0,
        KEY = 1,
        METER = 2,
        BARS = 3,
        TEMPO = 4,
        TRANSPOSE = 5,
        VOLUME = 6,
        PART = 7,
        TYPE = 8,
        INSTRUMENT = 9,
        CHORDS = 10,
        MELODY = 11,
        SWING = 12,
        BREAKPOINT = 13,
        COMPOSER = 14,
        COMMENTS = 15,
        STAVE = 16,
        TREBLE = 17,
        BASS = 18,
        GRAND = 19,
        AUTO = 20,
        NONE = 21,
        LAYOUT = 22,
        BASS_VOLUME = 23,
        DRUM_VOLUME = 24,
        CHORD_VOLUME = 25,
        STYLE = 26,
        SECTION = 27,
        BASS_INSTRUMENT = 28,
        PLAYBACK_TRANSPOSE = 29,
        SHOW_TITLE = 30,
        YEAR = 31,
        CHORD_FONT_SIZE = 32,
        PHRASE = 33,
        ROADMAP_LAYOUT = 34,
        ROADMAP = 36,
        VOICING_FILE = 36,
        MELODY_VOLUME = 37,
        UNKNOWN = 128
    };

    bool readLeadSheet(class Tokenizer &k, class Score &);
    bool readLeadSheet(class Tokenizer &, 
                        class Score &, 
                        bool overrideStaveType, 
                        Constants::StaveType useStaveType);
    void saveLeadSheet(class Score &, bool saveRoadMap, std::ostream &);
    std::string concatElements(class Polylist &);
    bool extractChordsAndMelody(class Polylist &chordsAndMelody,
                                       class Polylist &result);
    void addToMelodyPart(class Polylist &melodyInputReversed,
                        class MelodyPart &melody, 
                        int rise, int beatValue, 
                        class Key &key);
    bool populatePartWithChords(class ChordPart &chordProg,
                        class Polylist &chords,
                        int slotsAvailable,
                        int slotsPerBar);
    void addToChordPart(class Polylist &chordInputReversed, 
                        class ChordPart &chords,
                        int rise, int slotsPerBar, 
                        class Key &key);
    void addToMelodyFromPolylist(class Polylist &in, 
                        class MelodyPart &melody,
                        int rise, int slotsPerBeat,
                        class Key &key);
    int lookup(std::string &arg, std::vector<std::string> &table);
    void classifyNotes(class MelodyPart& melody, class ChordPart& chords);

};