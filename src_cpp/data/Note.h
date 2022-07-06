#pragma once

#include "IUnit.h"
#include "Constants.h"

/**
 * The Note class represents a note in a melody.
 * It stores the rhythm value, the MIDI pitchName number, and a flag
 * that indicates the accidental on the note.  
 * The Impro-Visor ref also includes drawing clues, but currently
 * that's out of scope for this port.
 */
class Note : public IUnit
{
public:
    static const int UNDEFINED_PITCH;
    static const int REST_PITCH;
    static int DEFAULT_PITCH;
    static int DEFAULT_VOLUME; // 0-127
    static int maxRhythmValuePerLine;

    static Note *MakeNote(int pitch, int dur);
    static Note *MakeRest(int dur=DEFAULT_RHYTHM_VALUE);
    static bool isBlack(int pitch);
    static Constants::Accidental getSharpOrFlat(bool x);
    static Note getClosestMatch(int pitch, class Polylist tonesPL);

    /* -------------------------------------------------- */
    using NotePtr = std::shared_ptr<Note>;

    Note(Note const &rhs);
    Note(int pitch, Constants::Accidental accidental, 
        int rhythmValue=DEFAULT_RHYTHM_VALUE);
    Note(int pitch, bool natural, bool sharp, int dur);
    Note(int pitch, bool sharp, int dur=DEFAULT_RHYTHM_VALUE);
    Note(int pitch, int dur=DEFAULT_RHYTHM_VALUE);

    void setRhythmValue(int r) override { m_rhythmValue = r; }
    int getRhythmValue() const override { return m_rhythmValue; }
    IUnit *copy() const override;
    bool isActive() const override { return m_pitch != REST_PITCH; }
    std::string toString() const override;
    UnitType getUnitType() const override { return m_pitch == REST_PITCH ? UnitType::k_Rest : UnitType::k_Note; }

    std::string getPitchClassName() const;
    bool isBlack() const;
    bool isValid() const { return m_pitch != UNDEFINED_PITCH; }
    void setVolume(int v); // 0-127
    int getVolume() const { return m_volume; }
    bool equalsBasic(Note const &rhs) const;
    int getPitch() const { return m_pitch; }
    int setPitch(int p) { m_pitch = p; }
    bool samePitch(Note const &rhs) const;
    bool adjacentPitch(Note const &rhs) const;
    void setAccidentalFromPitch();
    void setAccidental(Constants::Accidental acc);
    Constants::Accidental getAccidental() const { return m_accidental; }
    bool isAccidentalInKey(int keySig);
    bool toggleEnharmonic();

protected: // shared by Rest subclass
    int m_rhythmValue;

private:
    int m_pitch;
    int m_volume;
    Constants::Accidental m_accidental;

    static int s_accumulatedRhythmValue;

};