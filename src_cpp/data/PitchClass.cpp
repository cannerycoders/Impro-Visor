#include "PitchClass.h"
#include "Constants.h"
#include "util/String.h"

#include <string>
#include <cassert>

using namespace Constants;

PitchClass::PitchClass(char const *name, int index, 
    int semis,
    bool nat, bool shrp, bool shrpPref,
    char const *cbase)
{
    m_name = name;
    m_index = index;
    m_semitonesAboveC = semis;
    m_natural = nat;
    m_sharp = shrp;
    m_sharpPreference = shrpPref;
    m_chordBase = name;
}

PitchClass const PitchClass::sInvalid("", -1, 0, true, false, false, "");
PitchClass const PitchClass::cClass("c", 8, 0, true, false, false, "C");
PitchClass const PitchClass::pitchClass[] = 
{
    PitchClass("fb",  0,  4, false, false, false, "E"),
    PitchClass("cb",  1, 11, false, false, false, "B"),
    PitchClass("gb",  2,  6, false, false, false, "Gb"),
    PitchClass("db",  3,  1, false, false, false, "Db"),
    PitchClass("ab",  4,  8, false, false, false, "Ab"),
    PitchClass("eb",  5,  3, false, false, false, "Eb"),
    PitchClass("bb",  6, 10, false, false, false, "Bb"),
    PitchClass("f",   7,  5,  true, false, false, "F"),
    cClass,
    PitchClass("g",   9,  7,  true, false, true,  "G"), 
    PitchClass("d",  10,  2,  true, false, true,  "D"),
    PitchClass("a",  11,  9,  true, false, true,  "A"),
    PitchClass("e",  12,  4,  true, false, true,  "E"),
    PitchClass("b",  13, 11,  true, false, true,  "B"),
    PitchClass("f#", 14,  6, false, true,  true,  "F#"),
    PitchClass("c#", 15,  1, false, true,  true,  "C#"),
    PitchClass("g#", 16,  8, false, true,  false, "Ab"),
    PitchClass("d#", 17,  3, false, true,  false, "Eb"),
    PitchClass("a#", 18, 10, false, true,  false, "Bb"),
    PitchClass("e#", 19,  5, false, true,  false, "F"),
    PitchClass("b#", 20,  0, false, true,  false, "C")
};

const int 
    PitchClass::fb = 12,  // use e rather than fb
    PitchClass::cb = 13,  // use b rather than cb
    PitchClass::gb = 2,
    PitchClass::db = 3,
    PitchClass::ab = 4,
    PitchClass::eb = 5,
    PitchClass::bb = 6,
    PitchClass::f  = 7,
    PitchClass::c  = 8,
    PitchClass::g  = 9,
    PitchClass::d  = 10,
    PitchClass::a = 11,
    PitchClass::e = 12,
    PitchClass::b = 13,
    PitchClass::fs = 14,
    PitchClass::cs = 15,
    PitchClass::gs = 16,
    PitchClass::ds = 5,   // use bb rather than d#
    PitchClass::as = 6,   // use bb rather than a#
    PitchClass::es = 7,   // use f rather than e#
    PitchClass::bs = 8;   // use c rather than b#

const int PitchClass::indexFromSemitones[] = 
  {  c, db, d, eb, e,  f, gb, g, ab, a, bb, b, c, cs, d, ds,  e, f, fs, g, gs, a, as,  b, bs };
//{  c, db, d, eb, fb,  f, gb, g, ab, a, bb, cb, c, cs, d, ds,  e, es, fs, g, gs, a, as,  b, bs};

// In the table below, columns are tonalities, while rows correspond to transpositions of
// the tonality by some number of semitones (0-11).  This is an attempt to get sharps and
// flats to be coordinated when transposing.  We don't want to see sharps and flats mixed
// in the same column.

typedef PitchClass PC;

const int PC::upTranspositions[][25] = 
{
  /*  0 */ {PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es, PC::bs},
  /*  1 */ { PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs},
  /*  2 */ {PC::fs, PC::cs, PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d},
  /*  3 */ { PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds},
  /*  4 */ {PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d,  PC::a,  PC::e},
  /*  5 */ { PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es},
  /*  6 */ {PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as,  PC::es, PC::bs,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs},
  /*  7 */ { PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g},
  /*  8 */ { PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs},
  /*  9 */ {PC::cs, PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d,  PC::a},
  /* 10 */ { PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as},
  /* 11 */ {PC::ds, PC::as,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es,  PC::bs,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b}
};

const int PC::downTranspositions[][25] = 
{
  /*  0 */ {PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::fs, PC::cs, PC::gs, PC::ds, PC::as, PC::es, PC::bs},
  /*  1 */ {PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b},
  /*  2 */ { PC::d,  PC::a, PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb},
  /*  3 */ {PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a},
  /*  4 */ { PC::c,  PC::g,  PC::d,  PC::a, PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab},
  /*  5 */ {PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g},
  /*  6 */ {PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb},
  /*  7 */ { PC::a, PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f},
  /*  8 */ {PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::f},
  /*  9 */ { PC::g,  PC::d,  PC::a, PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb},
  /* 10 */ {PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d},
  /* 11 */ { PC::f,  PC::c,  PC::g,  PC::d,  PC::a, PC::fb, PC::cb, PC::gb, PC::db, PC::ab, PC::eb, PC::bb,  PC::f,  PC::c,  PC::g,  PC::d,  PC::a,  PC::e,  PC::b, PC::gb, PC::db}
};

const int PC::BOTTOM_PITCH = 0;
const int PC::TOP_PITCH = 20;
const int PC::PITCH_NAME_SIZE = 21;

/**
 * Numbers used for key-independent pitches (e.g. in cell display).
 */

const std::string PC::numbers[] = 
{
"3", 
"7", 
"b5",
"b2",
"b6",
"b3",
"b7",
"4", 
"1", 
"5", 
"2", 
"6", 
"3", 
"7", 
"#4",
"#1",
"#5",
"#2",
"#6",
"4", 
"1"  
};

const std::string PC::romanNumerals[] = 
{
"bIV", 
"bI", 
"bV", 
"bII", 
"bVI", 
"bIII", 
"bVII", 
"IV", 
"I", 
"V", 
"II", 
"VI", 
"III", 
"VII", 
"#IV", 
"#I", 
"#V", 
"#II", 
"#VI", 
"#III", 
"#VII"
};

/* ---------------------------------------------------------------- */
std::string PC::upperCaseNote(std::string &s)
{
    return UtString::toUpper(s);
}

bool PC::isValidPitch(std::string const &name)
{
  return &getPitchClass(name) != &sInvalid;
}

bool PC::isValidPitchStart(char c)
{
    if(c >= 'a' && c <= 'g')
        return true;
    else
        return false;
}

PitchClass const &PC::getPitchClass(std::string const &name)
{
    if(name.length() == 0) return sInvalid;
    std::string lcName = UtString::toLower(name);
    int index = -1;
    switch(lcName[0])
    {
    case 'f': index = 7; break;
    case 'c': index = 8; break;
    case 'g': index = 9; break;
    case 'd': index = 10; break;
    case 'a': index = 11; break;
    case 'e': index = 12; break;
    case 'b': index = 13; break;
    }
    if(lcName.length() > 1)
    {
        switch(lcName[1])
        {
        case 'b': index -= 7; break;
        case '#': index += 7; break;
        }
    }
    if(index < 0)
        return sInvalid;
    else
        return pitchClass[index];
}

PitchClass const &PC::getPitchClass(int index)
{
    assert(index >= BOTTOM_PITCH && index <= TOP_PITCH);
    return pitchClass[index];
}

/**
 * Get a representative PitchClass given the midi note number.
 * The PitchClass of a midi note is not unique, so we choose
 * in the range gb to b on the line of fifths
 */
PitchClass const &PC::getPitchClassFromMidi(int midi)
{
    while(midi < 0)
        midi += OCTAVE;
    return pitchClass[8+((midi*7)%OCTAVE)];
}

/**
 * Find delta in line of fifths between two pitches in table.
 * (This might not be needed given findRise below.)
 * The pitches are specified by strings which should be names of pitches.
 */
int PC::findDelta(std::string &from, std::string &to)
{
    PitchClass fromClass = getPitchClass(from);
    PitchClass   toClass = getPitchClass(to);
    assert( &fromClass != &sInvalid );
    assert( &toClass != &sInvalid );
    return toClass.getIndex() - fromClass.getIndex();
}

PitchClass const &PC::transpose(PitchClass const &pc, int semitones)
{
    return pc.transpose(semitones);
}

/**
 * Find rise in semitones between two pitches in table, for purpose of 
 * transposing other pitches by the same interval as between the given two pitches.
 * The pitches are specified by strings which should be names of pitches.
 */
int PC::findRise(std::string &from, std::string &to)
{
    PitchClass const &fromClass = getPitchClass(from);
    PitchClass const &toClass = getPitchClass(to);
    assert( &fromClass != &sInvalid );
    assert( &toClass != &sInvalid );
    return findRise(fromClass, toClass);
}

int PC::findRise(PitchClass const &fromClass, PitchClass const &toClass)
{
    int rise = toClass.getSemitones() - fromClass.getSemitones();
    if(rise >= OCTAVE)
    {
        rise = rise%OCTAVE;
    }
    else 
    if(rise < 0)
    {
        rise = OCTAVE - ((-rise)%OCTAVE);
    }

    // By making the rise negative, transposition will take this to 
    // a scale preferring flats
    if(rise > 0 && !toClass.m_sharpPreference)
    {
        rise = - (OCTAVE - rise);
    }
    else 
    if(rise < 0 && toClass.m_sharpPreference)
    {
        rise = - (OCTAVE + rise);
    }
    //System.out.println("rise from " + from + " to " + to + " = " + rise);
    return rise;
}

/**
 * Find rise in semitones between C and a pitch in table, for purpose 
 * of transposing other pitches by the same interval.
 * The pitches is specified by a String in this version.
 */
int PC::findRise(PitchClass const &toClass)
{
    return findRise(cClass, toClass);
}

/**
 * Find rise in semitones between C and a pitch in table, for purpose 
 * of transposing other pitches by the same interval.
 * The pitches is specified by a String in this version.
 */
int PC::findRise(std::string &to)
{
    PitchClass toClass = getPitchClass(to);
    assert( &toClass != &sInvalid );
    return findRise(toClass);
}

/**
 * Find rise in semitones between a pitch and C in table, for purpose of 
 * transposing other pitches by the same interval.
 * The pitches is specified by a String in this version.
 */
int PC::findRiseToC(PitchClass const &from)
{
    return findRise(from, cClass);
}

int PC::findRiseToC(std::string &from)
{
    PitchClass fromClass = getPitchClass(from);
    assert(&fromClass != &sInvalid);
    return findRiseToC(fromClass);
}

bool PC::enharmonic(std::string &x, std::string &y)
{
    return getPitchClass(x).enharmonic(getPitchClass(y));
}

/**
 * Make a note from a PitchClass name.  The specific pitch for the note
 * is found by using the midiBase argument as C that beings the octave in
 * which the desired note occurs.
 *
 * If there is a problem with the PitchClass name, Note.sInvalid is 
 * returned.
 */
Note PC::makeNote(PitchClass const &pc, int midiBase, int duration)
{
    return makeNoteAbove(pc, midiBase, 0, duration);
}

Note PC::makeNote(std::string &pcName, int midiBase, int duration)
{
    return makeNoteAbove(pcName, midiBase, 0, duration);
}

Note PC::makeNoteAbove(std::string &pcName, int midiBase, int minimum, int duration)
{
  PitchClass const &pc = getPitchClass(pcName);
  return makeNoteAbove(pc, midiBase, minimum, duration);
}

Note PC::makeNoteAbove(PitchClass const &pc, int midiBase, int minimum, int duration)
{
    int midi = midiBase + pc.getSemitones();
    while(midi < minimum)
        midi += OCTAVE;
    bool natural = pc.getNatural();
    bool sharp = pc.getSharp();
    return Note(midi, natural, sharp, duration);  
}

std::string const &PC::keyToRomanNumerals(PitchClass const &current,
                                        PitchClass const &home)
{
    return current.keyToRomanNumerals(home);
}

std::string const &PC::keyToRomanNumerals(std::string &current,
                                            std::string &home)
{
    return keyToRomanNumerals(getPitchClass(current),
                getPitchClass(home));
}

/* --instance methods -------------------------------------------- */
PitchClass const &
PC::transpose(int semitones) const 
{
    int newIndex;
    if(semitones >= 0)
    {
        semitones = semitones%12;
        if(semitones == 0)
            return *this;
        newIndex = upTranspositions[semitones][m_index];
    }
    else
    {
        semitones = (-semitones)%12;
        if(semitones == 0)
            return *this;
        newIndex = downTranspositions[semitones][m_index];
    }
    return pitchClass[newIndex];
}

/**
 * enharmonic determines whether this pitch classes
 * is enharmonically equivalent to the argumen
 */
bool PC::enharmonic(PitchClass const &other) const
{
    return m_semitonesAboveC == other.m_semitonesAboveC;
}

bool PC::enharmonic(int otherIndex) const
{
    return (m_semitonesAboveC % 12) == (otherIndex % 12);
}

std::string const &PC::keyToRomanNumerals(PitchClass const &homeKey) const
{
    int index = transpose(findRiseToC(homeKey)).getIndex();
    return romanNumerals[index];
}