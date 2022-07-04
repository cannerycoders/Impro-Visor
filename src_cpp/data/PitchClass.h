#pragma once

#include "Note.h"

#include <vector>
#include <string>

class PitchClass
{
private:
    // construct via factory method
    PitchClass(char const *name, int index, int semitonesAboveC,
        bool natural, bool sharp, bool sharpPref,
        char const *chordBase);

public:
    static PitchClass const cClass;
    static PitchClass const pitchClass[];
    static PitchClass const sInvalid;

    static const int 
        fb,  // use e rather than fb
        cb,  // use b rather than cb
        gb,
        db,
        ab,
        eb,
        bb,
        f,
        c,
        g,
        d,
        a,
        e,
        b,
        fs,
        cs,
        gs,
        ds,   // use bb rather than d#
        as,   // use bb rather than a#
        es,   // use f rather than e#
        bs;   // use c rather than b#
    static const int indexFromSemitones[25];
    static const int upTranspositions[][25];
    static const int downTranspositions[][25];
    static const std::string numbers[];
    static const std::string romanNumerals[];

    static std::string upperCaseNote(std::string &s);
    static bool isValidPitch(std::string &s);
    static bool isValidPitchStart(char c);
    static PitchClass const &getPitchClass(std::string &);
    static PitchClass const &getPitchClass(int index);
    static PitchClass const &getPitchClassFromMidi(int midi);
    static int findDelta(std::string &from, std::string &to);
    static PitchClass const &transpose(PitchClass const &pc, int semitones);
    static int findRise(std::string &from, std::string &to);
    static int findRise(PitchClass const &to);
    static int findRise(std::string &to);
    static int findRiseToC(PitchClass const &from);
    static int findRiseToC(std::string &from);
    static bool enharmonic(PitchClass const &other);
    static bool enharmonic(int otherIndex);
    static bool enharmonic(std::string &x, std::string &y);
    static Note makeNote(PitchClass const &, int midiBase, int duration);
    static Note makeNote(std::string &, int midiBase, int duration);
    static Note makeNoteAbove(std::string &, int midiBase, int minimum, int duration);
    static Note makeNoteAbove(PitchClass const &, int midiBase, int minimum, int duration);
    static std::string const &keyToRomanNumerals(PitchClass const &);
    static std::string const &keyToRomanNumerals(PitchClass const &current,
                                                PitchClass const &home);
    static std::string const &keyToRomanNumerals(std::string &current,
                                                std::string &home);

private:
    static const int BOTTOM_PITCH;
    static const int TOP_PITCH;
    static const int PITCH_NAME_SIZE;

public: /* instance method */
    int getIndex() { return m_index; }
    int getSemitones() { return m_semitonesAboveC; }
    int getNatural() { return m_natural; }
    int getSharp() { return m_sharp; }
    std::string const & getChordBase() { return m_chordBase; }
    std::string const & toString() { return m_name; }

private:
    std::string m_name;
    int m_index;
    int m_semitonesAboveC;
    bool m_natural;
    bool m_sharp; // if !natural, then either sharp or flat
    bool m_sharpPreference; // whether sharp accidentals are preferred for corresponding major scale.
    std::string m_chordBase;
};