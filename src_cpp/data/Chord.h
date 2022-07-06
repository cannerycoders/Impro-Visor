#pragma once

#include "IUnit.h"

#include "Constants.h"
#include "ChordForm.h"
#include "ChordSymbol.h"
#include "PitchClass.h"

class Chord : public IUnit
{
public:
    static int maxBarsOnLine;
    static int maxNotes;

    void setRhythmValue(int r) override { m_rhythmValue = r; }
    int getRhythmValue() const override { return m_rhythmValue; }
    IUnit *copy() const override;
    std::string toString() const override;
    UnitType getUnitType() const override { return UnitType::k_Chord; }

public:
    static Chord *makeChord(std::string &nm, int rv=DEFAULT_RHYTHM_VALUE);
    static void initSaveToLeadSheet();
    static void flushChordBuffer(std::ostream &out, int metre[], 
                            bool lineBreaks, bool finalBar);
    static Chord *open(std::istream &in);

public:
    Chord(std::string const &nm, int rhythmValue=DEFAULT_RHYTHM_VALUE);
    Chord(ChordSymbol &sym, int rhythmValue=DEFAULT_RHYTHM_VALUE);
    Chord(int rhythmValue=DEFAULT_RHYTHM_VALUE);
    // Chord(ChordBlock &block); (WIP from brickdictionary)

    void incRhythmValue(int rhythmValue);
    bool toggleEnharmonic();
    void transpose(int semitones);
    std::string const &getName();
    ChordForm const &getChordForm();
    std::string const &getFamily();
    Polylist getSpell();
    Polylist getColor();
    Polylist getFirstScale();
    Polylist getScales();
    std::string isValidScale(std::string &type);
    Polylist getPriority();

    void setName(std::string nm);
    std::string getRoot();
    PitchClass getRootPitchClass();
    int getTypeIndex(Note &n);
    void setVoicing(Polylist voicing);
    Polylist const &getVoicing() { return m_voicing; }
    ChordSymbol const &getChordSymbol();
    std::string const &getQuality();
    bool isNOCHORD();
    int getRootSemitones();

    /**
     * Return the type of note relative to a this and the next note and chord.
     * Intended for use in bass lines, this classification prefers approach tone
     * over color tone, but that can be changed.
     */
    int classify(Note &n, Note &nextNode, Chord &nextChord);

    /**
     * Writes the Chord to the passed ostream in Leadsheet notation.
     * @param out       the BufferedWriter to write the Chord to
     */
    void saveLeadsheet(std::ostream &out, int metre[], bool lineBreaks=true);

#if 0
    /**
     * Adds the Chord at the specified time on the specified Track and channel
     * in the specified Sequence, then returns the time that a sequential
     * Chord should be added.
     * @param seq       the Sequence to add the Chord to
     * @param time      the time to start the Chord at
     * @param ch        the channel to put the Chord on
     * @return long      the time that a sequential Chord should start
     */
    long render(MidiSequence seq, 
                long time, 
                int ch, 
                Style style, 
                Chord prev, 
                int rhythmValue, 
                int transposition, 
                int endLimitIndex);
#endif

private:
    ChordSymbol m_symbol;
    Polylist m_voicing;
    int m_rhythmValue;
};