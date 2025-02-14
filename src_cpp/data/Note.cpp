#include "Note.h"
#include "polya/Polylist.h"

const int Note::REST_PITCH = Constants::REST; // -1
const int Note::UNDEFINED_PITCH = -2;

int Note::DEFAULT_PITCH = 60;
int Note::DEFAULT_VOLUME = 127;
int Note::maxRhythmValuePerLine = 480; // 4 beats, public
int Note::s_accumulatedRhythmValue = 0; // private

static bool s_black[] = 
{
    false, true, false, true, false, false, 
    true, false, true, false, true, false
};

/** 
 * Table used to give names of notes in the case that the accidental is flat.
 */
static char const *s_flatPitchFromMidi[] =
{
    "c", "db", "d", "eb", "fb", "f", "gb", "g", "ab", "a", "bb", "cb"
};

static char const *s_sharpPitchFromMidi[] =
{
    "b#", "c#", "d", "d#", "e", "e#", "f#", "g", "g#", "a", "a#", "b"
};

static char const *s_naturalPitchFromMidi[] =
{
    "c", "c", "d", "d", "e", "f", "f", "g", "g", "a", "a", "b"
};

/** 
 * Table used by method isAccidentalInKeyto determine which pitches
 * are accidental in sharp keys.
 */
static int s_sharpInKeysAbove[] =
{
    6, 1, 9, 3, 5, 9, 0, 9, 2, 9, 4, 9
};	// 9 is like infinity

/** 
 * Table used by method isAccidentalInKeyto determine which pitches
 * are accidental in flat keys.
 */
static int s_flatInKeysBelow[] =
{
    9, 4, 9, 2, 7, 9, 5, 9, 3, 9, 1, 6
};	// 9 is like infinity
//c  db d  eb fb f  gb g  ab a  bb cb

/*static*/ bool Note::isBlack(int pitch)
{
    while(pitch < 0)
        pitch += Constants::OCTAVE;
    return s_black[pitch % 12];
}

/*static*/ Note
Note::makeNote(int pitch, int dur)
{
    return Note(pitch, dur);
}

/*static*/ Note::NotePtr
Note::makeNotePtr(int pitch, int dur)
{
    return std::make_shared<Note>(pitch, dur);
}

/*static*/ Note
Note::makeRest(int dur)
{
    return Note(REST_PITCH, Constants::Accidental::NOTHING, dur);
}

/*static*/ Note::NotePtr
Note::makeRestPtr(int dur)
{
    return std::make_shared<Note>(REST_PITCH, Constants::Accidental::NOTHING, dur);
}

/*static*/Constants::Accidental
Note::getSharpOrFlat(bool x)
{
  return x ? Constants::Accidental::SHARP : Constants::Accidental::FLAT;
}

/**
 * Gets the closest match Note to a given pitchName, from a polylist of NoteSymbols
 * @param pitch
 * @param tonesPL
 * @return Note  an instance of the 'closest' note in list from the pitchName
 */
/*static*/ Note 
Note::getClosestMatch(int pitch, Polylist tonesPL)
{
#if 0
    int originalPitch = pitch;
    if(!tonesPL.nonEmpty())
    {
        ErrorLog.log(ErrorLog.WARNING, 
            "*** Error: No tones list to match against.");
    }

    Polylist list = tonesPL;
    int[] tones = new int[list.length()];

    /* Make an array of pitches of all acceptable tones to match to */
    for(int i = 0; list.nonEmpty(); i++)
    {
        try
        {
            tones[i] = ((NoteSymbol)list.first()).getMIDI() % OCTAVE;
            list = list.rest();
        }
        catch( Exception ep )
        {
            //ep.printStackTrace();
        }
    }

    /* Search incrementally within the tones list for the 'closest'
    * acceptable pitchName.  This will fit our contour best.
    */
    int stepSearch = 0;
    int indexMatch;

    while((indexMatch = arrayContains((pitch % OCTAVE), tones)) == OUT_OF_BOUNDS)
    {
        stepSearch++;
        if(stepSearch % 2 == 0 )
        {
            pitch -= stepSearch;
        }
        else
        {
            pitch += stepSearch;
        }
    }

    Note note = ((NoteSymbol)tonesPL.nth(indexMatch)).toNote();
    note.setPitch(pitch);
    /* if( pitchName != originalPitch )
    {
        System.out.println("closest match to " + new Note(originalPitch).toLeadsheet() + " is " + note.toLeadsheet() + " among " + tonesPL);
    }
    */
    return note;
#else
    return Note(60);
#endif
}

inline int accumulateValue(int value, int duration, 
    std::string &buf, char const *v)
{
    while(duration >= value)
    {
        buf.append(v);
        duration -= value;
    }
    return duration;
}

inline int accumulateExactValue(int value, int duration, 
    std::string &buf, char const *v)
{
    if(duration % value == 0)
        return accumulateValue(value, duration, buf, v);
    else
        return duration;
}

/**
 * @brief  convert incoming duration to a string representation
 * @returns the residue (error)
 */

/*static*/int
Note::getDurationString(int value, std::string &result)
{
    // Try decomposing in two different orders, then pick the shorter description
    std::string buf1, buf2;
    int saved_value = value;

    // First decomposition
    value = accumulateValue(Constants::WHOLE, value, buf1, "+1");
    value = accumulateValue(Constants::HALF, value, buf1, "+2");
    value = accumulateValue(Constants::QUARTER, value, buf1, "+4");
    value = accumulateExactValue(Constants::QUARTER_QUINTUPLET, value, buf1, "+4/5");
    value = accumulateValue(Constants::EIGHTH, value, buf1, "+8");
    value = accumulateExactValue(Constants::EIGHTH_QUINTUPLET, value, buf1, "+8/5");
    value = accumulateValue(Constants::SIXTEENTH, value, buf1, "+16");
    value = accumulateExactValue(Constants::SIXTEENTH_QUINTUPLET, value, buf1, "+16/5");
    value = accumulateValue(Constants::THIRTYSECOND, value, buf1, "+32");
    value = accumulateExactValue(Constants::THIRTYSECOND_QUINTUPLET, value, buf1, "+32/5");
    value = accumulateValue(Constants::HALF_TRIPLET, value, buf1, "+2/3");
    value = accumulateValue(Constants::QUARTER_TRIPLET, value, buf1, "+4/3");
    value = accumulateValue(Constants::EIGHTH_TRIPLET, value, buf1, "+8/3");
    value = accumulateValue(Constants::SIXTEENTH_TRIPLET, value, buf1, "+16/3");
    value = accumulateValue(Constants::THIRTYSECOND_TRIPLET, value, buf1, "+32/3");

    // To make any residual concise
    value = accumulateValue(Constants::SIXTIETH, value, buf1, "+60");
    value = accumulateValue(Constants::ONETWENTIETH, value, buf1, "+120");
    value = accumulateValue(Constants::TWOFORTIETH, value, buf1, "+240");
    value = accumulateValue(Constants::FOUREIGHTIETH, value, buf1, "+480");
    int residue1 = value;

    // Second decomposition
    value = saved_value;
    value = accumulateValue(Constants::HALF_TRIPLET, value, buf2, "+2/3");
    value = accumulateValue(Constants::QUARTER_TRIPLET, value, buf2, "+4/3");
    value = accumulateValue(Constants::EIGHTH_TRIPLET, value, buf2, "+8/3");
    value = accumulateValue(Constants::SIXTEENTH_TRIPLET, value, buf2, "+16/3");
    value = accumulateValue(Constants::THIRTYSECOND_TRIPLET, value, buf2, "+32/3");
    value = accumulateValue(Constants::WHOLE, value, buf2, "+1");
    value = accumulateValue(Constants::HALF, value, buf2, "+2");
    value = accumulateValue(Constants::QUARTER, value, buf2, "+4");
    value = accumulateValue(Constants::EIGHTH, value, buf2, "+8");
    value = accumulateValue(Constants::SIXTEENTH, value, buf2, "+16");
    value = accumulateValue(Constants::THIRTYSECOND, value, buf2, "+32");
    
    // To make any residual concise
    value = accumulateValue(Constants::SIXTIETH, value, buf2, "+60");
    value = accumulateValue(Constants::ONETWENTIETH, value, buf2, "+120");
    value = accumulateValue(Constants::TWOFORTIETH, value, buf2, "+240");
    value = accumulateValue(Constants::FOUREIGHTIETH, value, buf2, "+480");
    int residue2 = value;
    
    int choice;  // This is the choice of which decomposition to use.
    // Both residues are 0, so make the choice based on length
    if(buf1.size() <= buf2.size())
        choice = 1;
    else
        choice = 2;
    switch( choice )
    {
    default:
    case 1:
        result.append(buf1);
        return residue1;

    case 2:
        result.append(buf2);
        return residue2;
    }
}

/* ---------------------------------------------------------------- */
Note::Note()
{
    m_pitch = UNDEFINED_PITCH;
}

Note::Note(Note const &rhs)
{
    m_pitch = rhs.m_pitch;
    m_accidental = rhs.m_accidental;
    m_rhythmValue = rhs.m_rhythmValue;
    m_volume = rhs.m_volume;
}

Note::Note(int pitch, Constants::Accidental accidental, int dur)
{
    m_pitch = pitch;
    m_accidental = accidental;
    m_rhythmValue = dur;
    m_volume = DEFAULT_VOLUME;
    // tied: false
    // firstTie: false
}

Note::Note(int pitch, bool natural, bool sharp, int dur)
{
    m_pitch = pitch;
    m_accidental = natural
            ? Constants::Accidental::NATURAL
            : getSharpOrFlat(sharp);
    m_rhythmValue = dur;
    m_volume = DEFAULT_VOLUME;
}

Note::Note(int pitch, bool sharp, int dur)
{
    m_pitch = pitch;
    m_accidental = getSharpOrFlat(sharp);
    m_rhythmValue = dur;
    m_volume = DEFAULT_VOLUME;
}

Note::Note(int pitch, int dur)
{
    m_pitch = pitch;
    m_accidental = isBlack(pitch%12) ?
            Constants::Accidental::SHARP :
            Constants::Accidental::NATURAL ;
    m_rhythmValue = dur;
    m_volume = DEFAULT_VOLUME;
}

IUnit *
Note::copy() const
{
    Note *c = new Note(*this);
    return c;
}

std::string
Note::toString() const
{
    std::string result;
    if(m_pitch == REST_PITCH)
    {
        result = "REST: [RhythmValue = ";
        result.append(std::to_string(m_rhythmValue));
        result.append("]");
    }
    else
    {
        result = "NOTE: [";
        result.append(getPitchClassName());
        result.append(", Pitch = ");
        result.append(std::to_string(m_pitch));
        // result.append(", ");
        // result.append(std::_toString(drawnPitch));
        result.append("][Accidental = ");
        result.append(std::to_string((int) m_accidental));
        result.append("][RhythmValue = ");
        result.append(std::to_string(m_rhythmValue));
        result.append("][volume = "); 
        result.append(std::to_string(m_volume));
        result.append("]");
    }
    return result;
}

std::string
Note::getPitchClassName() const
{
    std::string result;
    if(m_pitch == REST_PITCH)
        result = "r";
    else
    {
        int pitch_within_octave = m_pitch % Constants::OCTAVE;
        switch(m_accidental)
        {
        case Constants::Accidental::SHARP:
            result.append(s_sharpPitchFromMidi[pitch_within_octave]);
            break;
        case Constants::Accidental::FLAT:
            result.append(s_flatPitchFromMidi[pitch_within_octave]);
            break;
        default:
            result.append(s_naturalPitchFromMidi[pitch_within_octave]);
            break;
        }
    }
    return result;
}

bool 
Note::isAccidentalInKey(int keySig) const
{
    if(m_pitch >= 0)
    {
        if(keySig > 0)
        {
            return m_accidental == Constants::Accidental::FLAT //|| accidental == Accidental.NATURAL)  // See if this can be fixed.
              && keySig > s_sharpInKeysAbove[m_pitch % 12];
        }
        else
        if(keySig < 0)
        {
            return m_accidental == Constants::Accidental::SHARP // || accidental == Accidental.NATURAL) // See if this can be fixed.
              && keySig <= -s_flatInKeysBelow[m_pitch % 12];
        }
    }
    return false;
}

bool
Note::toggleEnharmonic()
{
    int ps = m_pitch % Constants::SEMITONES;
    if((ps == Constants::MODF && m_accidental == Constants::Accidental::SHARP) ||
       (ps == Constants::MODE && m_accidental == Constants::Accidental::FLAT) ||
       (ps == Constants::MODB && m_accidental == Constants::Accidental::FLAT) ||
       (ps == Constants::MODC && m_accidental == Constants::Accidental::SHARP) )
    {
        m_accidental = Constants::Accidental::NATURAL;
        return true;
    }
    else 
    if(m_accidental == Constants::Accidental::SHARP ||
       (m_accidental == Constants::Accidental::NATURAL && 
         (ps == Constants::MODE) || ps == Constants::MODB))
    {
        m_accidental = Constants::Accidental::FLAT;
        return true;
    }
    else 
    if(m_accidental == Constants::Accidental::FLAT ||
       m_accidental == Constants::Accidental::NATURAL && 
       (ps == Constants::MODF || ps == Constants::MODC) )
    {
        m_accidental = Constants::Accidental::SHARP;
        return true;
    }
    return false;
}

bool
Note::isBlack() const
{
    return isBlack(m_pitch);
}

void
Note::setVolume(int v)
{
    m_volume = v;
}

/**
 * Checks if the current note has the same pitch and duration as another note.
 */
bool
Note::equalsBasic(Note const &rhs) const
{
    return samePitch(rhs) && getRhythmValue() == rhs.getRhythmValue();
}

bool
Note::samePitch(Note const &rhs) const
{
    return getPitch() == rhs.getPitch();
}

bool
Note::adjacentPitch(Note const &rhs) const
{
    int thisPitch = getPitch();
    int otherPitch = rhs.getPitch();
    return thisPitch == otherPitch + 1 || thisPitch == otherPitch - 1;
}

void 
Note::setAccidentalFromPitch()
{
    if(m_pitch < 0)
        return;
    m_accidental = s_black[m_pitch % 12]
          ? Constants::Accidental::SHARP
          : Constants::Accidental::NATURAL;
}

void 
Note::setAccidental(Constants::Accidental acc)
{
    m_accidental = acc;
}

std::string
Note::toLeadsheet() const
{
    return std::string();
}