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
    Note(int midi, bool nat, bool sharp, int duration);

    enum
    {
        UNDEFINED = -2,
        DEFAULT_PITCH = 60
    };

    void setRhythmValue(int r) override { m_rhythmValue = r; }
    int getRhythmValue() override { return m_rhythmValue; }

protected: // shared by Rest subclass
    int m_rhythmValue;

private:
    int m_pitch;
    int m_volume;
    Constants::Accidental m_accidental;

};