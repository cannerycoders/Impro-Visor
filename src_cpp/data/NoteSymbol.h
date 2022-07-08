#pragma once

#include "MelodySymbol.h"
#include "VolumeSymbol.h"
#include "Constants.h"
#include "PitchClass.h"
#include "polya/Polylist.h"

#include <list>

/**
 * A NoteSymbol represents a note symbolically, and is typically created
 * from a leadsheet string.  A NoteSymbol can also represent a rest,
 * consistent with a Note doing so.  However, in contrast to class Note, 
 * it doesn't say anything about ties and such.
 * A Note can be created from a NoteSymbol using the toNote() method.
 */
class NoteSymbol : public MelodySymbol
{
public:
    static char const *DEFAULT_PITCH_SYMBOL;
    /**
     * Creates a NoteSymbol from a leadsheet String.
     * Because the String could be ill-formed, we use a factory rather than a constructor
     * and keep the actual constructor private.
     */
    using NSPtr = std::shared_ptr<NoteSymbol>;
    using NSList = std::list<NSPtr>; // for enhDrop, et al
    using PSList = std::vector<std::string>;

    static bool isValidNote(std::string const &s)
    {
        return makeNoteSymbol(s, 0).get() ? true : false;
    }

    static Note toNote(std::string const &str)
    {
        NSPtr p = makeNoteSymbol(str);
        if(p)
            return p->toNote();
        else
            return Note();
    }

    static NSPtr makeNoteSymbol(std::string string)
    {
        return makeNoteSymbol(string, 0);
    } 

    static NSPtr makeNoteSymbol(Note note)
    {
        return makeNoteSymbol(note.toLeadsheet());
    } 
   
    static NSPtr makeNoteSymbol(int midiValue)
    {
       return makeNoteSymbol(Note(midiValue));
    }

    static NSPtr makeNoteSymbol(int midiValue, int duration)
    {
        return makeNoteSymbol(Note(midiValue, duration));
    }

    static NSPtr makeNoteSymbol(std::string const &str, int transposition);

    static NSPtr getRestSymbol(int duration)
    {
        return std::make_shared<NoteSymbol>(PitchClass::sInvalid, 0, duration);
    }

    static int getDuration(Polylist &L);

    static NSList makeNoteSymbolList(Polylist &stringList, int rise=0);
    static NSList transposeNoteSymbolList(NSList &stringList, int rise);
    /**
     * @brief  modifies noteSymbols by removing all pitches that
     *  match the given PitchClass.
     */
    static void enhDrop(NSList &noteSymbols, PitchClass const &toDrop);
    static NSList closedVoicing(NSList &noteSymbols);
    static bool equalNoteSequences(NSList &x, NSList &y);
    static NSPtr getHighest(NSList &notes);
    static NSPtr getLowest(NSList &notes);

    static bool isomorphicNoteSequences(NSList &x, NSList &y);
    static PSList makePitchStringList(NSList &noteSymbolList);

    /* -------------------------------------------------------------------- */
    NoteSymbol(PitchClass const &pitchClass) :
        NoteSymbol(pitchClass, 0, Constants::BEAT/2)
    {
    } 
   
    NoteSymbol(NoteSymbol const &that) :
        NoteSymbol(that.m_pitchClass, 
                that.m_octave, that.m_duration, that.m_volume)
    {
    } 

    NoteSymbol(PitchClass const &pitchClass, int octave, int duration) :
        NoteSymbol(pitchClass, octave, duration, VolumeSymbol::k_maxVolume)
    {
    }

    NoteSymbol(PitchClass const &pitchClass, 
        int octave, int duration, int volume) :
        m_pitchClass(pitchClass)
    {
        m_octave = octave;
        m_duration = duration;
        m_volume = volume;
        m_probability = 1.f;
        updateStringRep();
    }

    std::string const &toString() { return m_stringRep; }

    int getOctave() const { return m_octave; }
    int getDuration() const { return m_duration; }
    PitchClass const& getPitchClass() const { return m_pitchClass; }
    int getPitchClassIndex() const { return m_pitchClass.getSemitones(); }
    std::string getPitchString() const { return m_pitchClass.toString(); }
    std::string getPitchOnly() const;
    std::string getDurationString() const;
    int getVolume() { return m_volume; }
    float getProbability() { return m_probability; }
    void setProbability(float p) { m_probability = p; }
    bool enharmonic(NoteSymbol &other);
    bool enharmonic(PitchClass const &other);
    int getRise(NoteSymbol &other);
    bool equalNotes(NoteSymbol &other);
    int getSemitonesAbove(NoteSymbol &other); // WIP?
    bool enhMember(NSList &noteSymbols);
    void enhDrop(NSList &noteSymbols);
    NSPtr findNextHigher(NSList &nsList);
    Note toNote();
    Note toNote(int volume);
    NSPtr transpose(int rise);
    bool isRest() const { return !m_pitchClass.isValid(); }
    void updateStringRep();
    int getSemitones() { return isRest() ? -1 : m_pitchClass.getSemitones(); }
    bool higher(NoteSymbol const &that) { return getMIDI() > that.getMIDI(); }
    int getMIDI() const;
    int getMIDIoctave() const;

private:

    /**
     * the PitchClass of this note.  A rest is represented by a pitchClass value of null.
     */
    PitchClass const &m_pitchClass;

    /**
     * The octave of this note, where the octave middle-C and just above is 0,
     * the octave above that is 1, below that is -1, etc.
     */
    int m_octave;

    /**
     * The duration of the note in slots
     */
    int m_duration;
    
    /**
     * Volume
     */
    int m_volume = VolumeSymbol::k_maxVolume;
    
    std::string m_stringRep;
    
    /**
     * used in lick generation
     */
    float m_probability;
};