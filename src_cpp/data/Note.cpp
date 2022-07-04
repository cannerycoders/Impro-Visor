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

/*static*/ bool Note::isBlack(int pitch)
{
    while(pitch < 0)
        pitch += Constants::OCTAVE;
    return s_black[pitch % 12];
}

/*static*/ Note
Note::MakeNote(int pitch, int dur)
{
    return Note(pitch, dur);
}

/*static*/ Note
Note::MakeRest(int dur)
{
    return Note(REST_PITCH, Constants::Accidental::NOTHING, dur);
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
        ErrorLog.log(ErrorLog.WARNING, 
            "*** Error: No tones list to match against.");

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

/* ---------------------------------------------------------------- */
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