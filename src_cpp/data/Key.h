#pragma once

#include "Constants.h"
#include "PitchClass.h"
#include "Note.h"
#include "polya/Polylist.h"

#include <string>

/**
 * The Key class deals with keys, their creation from strings, transposition, etc.
 * Keys are immutable objects and will be created only at initialization.
 */
class Key
{
public:
    static Key key[]; // table of all Keys organized as a line of fifths
    static Key &Ckey;
    enum KeyId
    {
        gbkey = 0, 
        BOTTOM_KEY = gbkey,
        dbkey = 1, 
        abkey = 2,  
        ebkey = 3, 
        bbkey = 4,  
        fkey = 5,  
        ckey = 6,  
        gkey = 7,   
        dkey = 8,  
        akey = 9,  
        ekey = 10, 
        bkey = 11, 
        fskey = 12, 
        cskey = 13,
        TOP_KEY = cskey,
        k_numKeys,
        KEY_SIZE=k_numKeys
    };
    enum PitchClassIndices
    {
        fb = 0,  
        BOTTOM_PITCH = fb,
        cb = 1,  
        gb = 2,  
        db = 3,  
        ab = 4,  
        eb = 5,  
        bb = 6,  
        f = 7,  
        c = 8,  
        g = 9,  
        d = 10,  
        a = 11,  
        e = 12,  
        b = 13,  
        fs = 14,  
        cs = 15,  
        gs = 16,  
        ds = 17,  
        as = 18,  
        es = 19,  
        bs = 20,
        TOP_PITCH=bs,
        k_numPCs,
        PITCH_NAME_SIZE=k_numPCs
    };
    enum
    {
        k_Octave = 12
    };
    static const int SEMITONEOFFSET; // 7

    static int cycleIndices[k_numKeys][k_Octave];
    static int chromaticIndices[k_numPCs][k_Octave];
    static int adjustPitchInKey[k_numKeys][k_Octave];
    static int transpositions[k_numKeys][k_Octave];

    static Key const &getKey(std::string &nm);
    static std::string const &getKeyName(int index);

    /**
     * Get the delta in the line of fifths, corresponding to a key
    * represented by a number of sharps (or negative for flats)
    * and a number of semitones transposition.
    */
    static int getKeyDelta(int sharps, int rise);
    static Polylist transposeChordList(Polylist &chordSeq, int rise);
    static Polylist transposeChordList(Polylist &chordSeq, int rise, Key const &key);
    static std::string transposeChord(std::string &chord, int rise, Key const &key);
    static std::string makeCroot(std::string &chord);
    /**
     * Change the root of the chord to specified note.
     */
    static std::string makeRoot(std::string &root, std::string &chord);
    static std::string getRoot(std::string &chord);
    static bool sameRoot(std::string &chord1, std::string &chord2);
    static bool isValidStem(std::string &stem);
    static bool hasValidStep(std::string &chord);
    static Polylist explodeChord(std::string &chord);
    static Polylist invalidNotes(Polylist &L);
    static int pitchFromLeadsheet(std::string &str, int rise=0);
    static Note noteFromLeadsheet(std::string &str, int rise, int slotsPerBeat);
    static Note noteFromLeadsheet(std::string &str, int rise, int slotsPerBeat,
                                Key const &);
    /**
     * Return a profile of a list of note Strings.
     */
    static std::string profileNoteStringList(Polylist &l, bool includeTrailer);
    /**
     * Transpose list of note Strings in leadsheet form by a certain rise
     * returning a list of note Strings.
    */
    static Polylist transposeNoteStringList(Polylist &L, int rise, 
                                            Key const &key);
    /**
     * Transpose a note String in leadsheet form by a certain rise
    * returning a String.  Returns null if the note String is not well-formed.
    */
    static std::string transposeNoteString(std::string &noteString, int rise, 
                                            Key const &key);
    /**
     * Transpose list of note Strings in leadsheet form by a certain rise
     * returning a list of numbers representing the notes independent of key.
     */
    static Polylist transposeNoteStringListToNumbers(Polylist &L, int rise,
                                                    Key const &key);
    /**
     * Transpose a note String in leadsheet form by a certain rise
    * returning a String representing a number, such as "3", "b5", "#2", etc.
    */
    static std::string transposeNoteStringToNumbers(std::string &noteString,
                                                    int rise, Key const &key);
    /*
    * transposeOne transposes one pitch class
    */
    static std::string transposeOne(std::string &from, std::string &to, 
                                std::string &p, Key const &key);

    /*
    * transposeList is a list version of transposePitch.
    * It transposes a whole list of pitches by the interval between
    * the from and to strings.
    */
    static Polylist transposeList(std::string &from, std::string &to, 
                            Polylist &L, Key const &key);
    /**
     * enharmonic determines whether the pitches represented by
    * two strings representing pitch are enharmonically equivalent
    */
    static bool enharmonic(std::string &x, std::string &y);
    /**
     * enMember determines whether the first pitch is enharmonically
    * equivalent to some member of a list
    */
    static bool enhMember(std::string &x, Polylist &L);
    /**
     * Auxiliary unit test method for Key.
     */
    static bool test(std::string &name);

    /**
     * Make a note from a pitch class name specified as one of the Strings
     * in the pitches table.  A pitch class represents many
     * individual pitches.  The specific pitch is found by
     * using the midiBase argument as C that beings the octave in
     * which the desired note occurs.
     *
     * If there is a problem with the String, null is returned.
     */
    static Note makeNote(std::string &pitchClassName, int midiBase,
                                int duration);
    /**
     * Make a note from a pitch class specified as one of the Strings
     * in the pitches table, above a minimum MIDI value.
     *  A pitch class represents many
     * individual pitches.  The specific pitch is found by
     * using the midiBase argument as C that beings the octave in
     * which the desired note occurs.  However, this octave can be
     * modified by specifying a non-zero minimum.  This is used in
     * arpeggios, for example, to guarantee the notes keep ascending,
     * by specifying the previous midi value.
     *
     * If there is a problem with the input, null is returned.
     */
    static Note makeNoteAbove(std::string &pitchClassName, int midiBase,
                            int minimum, int duration);

private:
    Key(int index, std::string &nm, int cPos);

public:
    /**
     * Render chromatic pitch as it would be in this key.
    */
    PitchClass const &renderInKey(std::string &name) const;
    /**
     * Defines how an offset of some number of semitones from the tonic of the key 
     * maps into pitch names.
     */
    PitchClass const &rep(int offset) const;
    int getIndex() const  { return m_index; }
    Key const &transpose(int semis);
    Key const &transpose(PitchClass const &, int semis);
    Key const &transposeKey(std::string &from, std::string &to);
    std::string toString();

private:
    int m_index; // unique id into table of known keys
    std::string m_name;
    int m_cPosition; // position of c in this key

};