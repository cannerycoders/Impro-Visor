#include "NoteSymbol.h"
#include "util/String.h"
#include "Key.h"
#include "Duration.h"
#include <cassert>

/* static --------------------------------------------------------------- */
char const *NoteSymbol::DEFAULT_PITCH_SYMBOL = "c+";

/* static */ NoteSymbol::NSPtr
NoteSymbol::makeNoteSymbol(std::string const &str, int transposition)
{
    size_t len = str.size();
    if(len == 0) return NoteSymbol::NSPtr();

    std::string lstr = UtString::toLower(str);

    // First determine if we have a rest
    char c = lstr[0];
    if(c == Constants::RESTCHAR)
    {
        return std::make_shared<NoteSymbol>(PitchClass::sInvalid, 0,
                                     Duration::getDuration(lstr.substr(1)));
    }
    else
    if(!PitchClass::isValidPitchStart(c))
       return NoteSymbol::NSPtr();

    int index = 1;
    bool natural = true;
    bool sharp = false;

    std::string noteBase;
    noteBase.push_back(c);
    if(index < len)
    {
        char second = lstr[1];
        if(second == Constants::SHARP || second == Constants::FLAT || second == 's')
        {
            index++;
            if(second == Constants::SHARP || second == Constants::FLAT) 
            {
                noteBase.push_back(second);
            } 
            else 
            {
                noteBase.push_back(Constants::SHARP);
            }
            natural = false;
            sharp = (second == Constants::SHARP || second == 's');
        }
    }

    PitchClass const &pc = PitchClass::getPitchClass(noteBase)
                                       .transpose(transposition);
    if(!pc.isValid())
        return NoteSymbol::NSPtr();

    int octave = 0;

    // Check for any octave shifts specified in the notation
    bool more = true;
    while(index < len && more)
    {
        switch(lstr[index])
        {
        case Constants::PLUS:
            octave++;
            index++;
            break;
        case Constants::MINUS:
            octave--;
            index++;
            break;
        case 'u':
            octave++;
            index++;
            break;
        default:
            more = false;
            break;
        }
    }
    int duration = Duration::getDuration(lstr.substr(index));
    return std::make_shared<NoteSymbol>(pc, octave, duration);
}

/*static*/ bool 
NoteSymbol::equalNoteSequences(NSList &x, NSList &y)
{
    if(x.size() == 0 || x.size() != y.size()) return false;
    auto xa = x.begin();
    auto ya = y.begin();
    auto xb = x.end();
    while(xa != xb)
    {
        auto xe = *xa;
        auto ye = *ya;
        if(!xe->equalNotes(*ye.get()))
            return false;
        xa++;
        ya++;
    }
    return true;
}
 
/*static*/ bool 
NoteSymbol::isomorphicNoteSequences(NSList &x, NSList &y)
{
    if(!x.size() || !y.size())
      return x.size() == y.size();
    
    NSPtr x0 = x.front();
    NSPtr y0 = y.front();

    int rise = x0->getRise(*(y0.get()));
    x = transposeNoteSymbolList(x, rise);
    return equalNoteSequences(x, y);
}

/*static*/void
NoteSymbol::enhDrop(NSList &noteSymbols, PitchClass const &toDrop)
{
    noteSymbols.remove_if([toDrop](const NSPtr p)
    {
        return p->enharmonic(toDrop);
    });
}

/*static*/ NoteSymbol::NSPtr 
NoteSymbol::getHighest(NSList &notes) 
{
    NSPtr highest;
    for(auto x : notes)
    {
        if(!highest || x->getMIDI() > highest->getMIDI())
            highest = x;
    }
    return highest;
}
  
/*static*/ NoteSymbol::NSPtr
NoteSymbol::getLowest(NSList &notes) 
{
    NSPtr lowest;
    for(auto x : notes)
    {
        if(!lowest || x->getMIDI() < lowest->getMIDI())
            lowest = x;
    }
    return lowest;
}

/**
 * Create a closed voicing ascending from the first note in the list.
 */
/*static*/ NoteSymbol::NSList 
NoteSymbol::closedVoicing(NSList &nsList)
{
    if(nsList.size() == 0)
        return NSList();

    // below, applying the constructor adjusts the octave
    size_t i = 0;
    NSList tmplist = nsList;
    NSList result;
    NoteSymbol &r = *tmplist.front();
    NSPtr front = std::make_shared<NoteSymbol>(r);
    front->enhDrop(tmplist);
    while(tmplist.size())
    {
        NSPtr next = front->findNextHigher(tmplist);
        if(!next) break;
        if(!next->enharmonic(*front))
            result.push_back(next);
        next->enhDrop(tmplist);
    }
    return result;
}

/**
 * Get duration of a list of NoteSymbols
 * except for trailing rests
 * returning an integer duration
 */
/*static*/ int 
NoteSymbol::getDuration(Polylist &L)
{
#if 0
    Polylist R = L.reverse();
    // ignore any rests at end (beginning of reverse)
    while(R.nonEmpty())
    {
        NoteSymbol noteSymbol = (NoteSymbol)R.first();
        if(!noteSymbol.isRest())
        {
            break;
        }
        R = R.rest();
    }

    int totalDuration = 0;
    while( R.nonEmpty() )
    {
        assert(R.first() instanceof NoteSymbol);
        totalDuration += ((NoteSymbol)R.first()).getDuration();
        R = R.rest();
    }
#else
    int totalDuration = 0;
    assert(0);
#endif
    return totalDuration;
}

/*static*/NoteSymbol::PSList 
NoteSymbol::makePitchStringList(NSList &noteSymbolList) 
{
    NoteSymbol::PSList ret;
    for(auto a : noteSymbolList)
        ret.push_back(a->getPitchString());
    return ret;
}

/**
 * Make a NoteSymbol list from a list of Strings.
 * As of 6/11/09, a pair (String Probability) can also be used,
 * e.g. to represent the user-declared probability of a color tone.
 @param stringList
 @param rise
 @return
 */
/*static*/ NoteSymbol::NSList
NoteSymbol::makeNoteSymbolList(Polylist &stringList, int rise)
{
    //System.out.print("makeNoteSymbolList " + stringList);
    NoteSymbol::NSList ret;
    auto a = stringList.getBegin();
    auto b = stringList.getEnd();
    while(a != b)
    {
        Polylist::ObjPtr ob = *a++;
        if(ob->getType() == PlistObj::k_list)
        {
            Polylist *L = ob->asListType();

            // Handle the option of specifying note probabilities in the
            // vocabulary
            if(L->size() == 2 
                && L->first()->isType(PlistObj::k_string) 
                && L->second()->isType(PlistObj::k_number))
            {
                std::string str = L->first()->asString();
                double prob = L->second()->asNumberType()->getFloat();
                NSPtr ns = makeNoteSymbol(str);
                if(ns) // Ignore invalid NoteSymbol
                {
                    ns = ns->transpose(rise);
                    ns->setProbability((float) prob);
                    ret.push_back(ns);
                }
            }
        }
        else
        {
            std::string str;
            if(ob->getType() == PlistObj::k_number)
            {
                // Make an integer into a note for subsequent use
                str = DEFAULT_PITCH_SYMBOL;
                str.append(ob->asString());
            } 
            else
            {
                assert(ob->getType() == PlistObj::k_string);
                str = ob->asString();
            }
            NSPtr ns = makeNoteSymbol(str);
            if(ns) // Ignore invalid NoteSymbol
            {
                ns = ns->transpose(rise);
                ret.push_back(ns);
            }
        }
    }
    //System.out.println(", rise = " + rise + ", bits = " + showContents(noteSymbolListToBitVector(R)));
    //System.out.println(", rise = " + rise + " to " + R.reverse());
    return ret;
}

/*static*/ NoteSymbol::NSList 
NoteSymbol::transposeNoteSymbolList(NSList &nslist, int rise)
{
    //System.out.print("transposeNoteSymbolList " + noteSymbolList);
    NSList nl;
    for(auto x : nslist)
    {
        nl.push_back(x->transpose(rise));
    }
    //System.out.println(", rise = " + rise + " to " + R.reverse());
    return nl;
}

/*static*/ NoteSymbol::PSList 
NoteSymbol::makeStringList(NSList &nsList, int rise)
{
    PSList result;
    for(auto x : nsList)
    {
        std::string s = x->transpose(rise)->toString();
        result.push_back(s);
    }
    return result;
}

/* ------------------------------------------------------------------------ */
/*
* Midi	Pitch class	Octave
*  0       c               -5
* 12       c               -4
* 24       c               -3
* 36       c               -2
* 48       c               -1
* 60       c                0
* 72       c                1  
* 84       c                2
* 96       c                3
* 108      c                4
* 120      c                5
* 127      g                5
*/

bool NoteSymbol::enharmonic(NoteSymbol &other)
{
    return getSemitones() == other.getSemitones();
}

bool NoteSymbol::enharmonic(PitchClass const &pc)
{
    return getSemitones() == pc.getSemitones();
}

int NoteSymbol::getRise(NoteSymbol &other)
{
    return other.getSemitones() - getSemitones();
}

bool NoteSymbol::equalNotes(NoteSymbol &other)
{
    return m_duration == other.m_duration 
        && m_octave == other.m_octave
        && enharmonic(other);
}

// Not sure that this does what is intended. USE CAUTION
/**
 * @return int number of semitones other is above this.
 *   (value is wrapped via OCTAVE to always be positive)
 */
int NoteSymbol::getSemitonesAbove(NoteSymbol &other)
{
    int distance = other.getSemitones() - getSemitones();
    while( distance <= 0 )
        distance += Constants::OCTAVE;
    return distance;
}

/**
 * Check to see whether the list argument contains a member that
 * is enharmonically equivalent to this member.
 */ 
bool NoteSymbol::enhMember(NSList &noteSymbols)
{
    for(auto x : noteSymbols)
    {
        if(enharmonic(*x) && x->getProbability() > 0) //true;
            return true;
    }
    return false;
}

void
NoteSymbol::enhDrop(NSList &noteSymbols)
{
    enhDrop(noteSymbols, m_pitchClass);
}

NoteSymbol::NSPtr 
NoteSymbol::findNextHigher(NSList &noteSymbols)
{
    // below, applying the constructor adjusts the octave
    int distance = 10000;
    NSPtr nextHigher;
    for(auto x : noteSymbols)
    {
        int d = getSemitonesAbove(*x);
        if(d != 0 && d < distance)
        {
            nextHigher = x;
            distance = d;
        }
    }
    return nextHigher;
}

Note 
NoteSymbol::toNote()
{
    return toNote(m_volume);
}

Note::NotePtr
NoteSymbol::toNotePtr()
{
    Note n = toNote(m_volume);
    return std::make_shared<Note>(n);
}
  
Note 
NoteSymbol::toNote(int volume)
{
    if(isRest())
        return Note::makeRest(m_duration);
    else
    {
        Note note = PitchClass::makeNote(m_pitchClass, getMIDIoctave(), m_duration);
        note.setVolume(m_volume);
        return note;
    }
}

NoteSymbol::NSPtr 
NoteSymbol::transpose(int rise)
{
    if(isRest() || rise == 0)
    {
        // no transposition for rest
        return std::make_shared<NoteSymbol>(*this);	
    }

    int newOctave = m_octave;
    int newSemitones = m_pitchClass.getSemitones() + rise;
    while(newSemitones >= 12)
    {
        newOctave++;
        newSemitones -= 12;
    }
    while(newSemitones < 0)
    {
        newOctave--;
        newSemitones += 12;
    }

    auto ret = std::make_shared<NoteSymbol>(m_pitchClass.transpose(rise), 
                    newOctave, m_duration);
    ret->setProbability(getProbability());
    return ret;
}

/**
 * @brief Get the Pitch Only object (eg: a++)
 */
std::string 
NoteSymbol::getPitchOnly() const
{
    std::string buffer0;
    if(isRest())
        buffer0.append(Constants::REST_STRING);
    else 
    {
        buffer0.append(m_pitchClass.toString());
        int octave = m_octave;
        while(octave >= 1)
        {
            buffer0.push_back(Constants::PLUS);
            octave--;
        }
        while(octave < 0)
        {
            buffer0.push_back(Constants::MINUS);
            octave++;
        }
    }
    return buffer0;
}

void 
NoteSymbol::updateStringRep()
{
    std::string buffer0 = getPitchOnly();
    std::string dur; 
    Note::getDurationString(m_duration, dur);
    buffer0.append(dur.substr(1));	// discard initial +
    m_stringRep = buffer0;
}

/**
 * Return the midi pitch number for this NoteSymbol
    */
int NoteSymbol::getMIDI() const
{
    if(isRest()) return -1;
    return m_pitchClass.getSemitones() + getMIDIoctave();
}

/**
 * Return the midi pitch number for this NoteSymbol
    */
int NoteSymbol::getMIDIoctave() const
{
    return Constants::CMIDI + 12*m_octave;
}

std::string NoteSymbol::getDurationString() const
{
    std::string dur;
    Note::getDurationString(m_duration, dur);
    return dur;
}


/**
 * Convert a list of NoteSymbols, transposed by a specified distance, to a
 * bitVector, i.e. an array of booleans
 * @param noteSymbolList
 * @param rise
 * @return an array of booleans representing the notes in the argument list
 *         after transposition by rise
 */
/*static*/ std::array<bool, Constants::OCTAVE>
NoteSymbol::makeBitVector(NSList &nsList, int rise)
{
    std::array<bool, Constants::OCTAVE> result;
    for(int i = 0; i < Constants::OCTAVE; i++)
        result[i] = false;
    for(auto x : nsList)
    {
        if(x->getPitchClass().isValid())
        {
            NSPtr transposed = x->transpose(rise);
            result[transposed->getPitchClassIndex()%Constants::OCTAVE] = true;
        }
    }
    return result;
  }


/*static*/ std::vector<int> 
NoteSymbol::makeMIDIarray(NSList &nsList, int rise)
{
    std::vector<int> result;
    result.reserve(nsList.size());
    for(auto x : nsList)
    {
        if(x->getPitchClass().isValid())
        {
            NSPtr transposed = x->transpose(rise);
            result.push_back(transposed->getMIDI());
        }
    }
    return result;
}

#define S Constants::Accidental::SHARP
#define F Constants::Accidental::FLAT
#define N Constants::Accidental::NATURAL

/*static*/ const Constants::Accidental 
NoteSymbol::accidentalByKey[Constants::NUMKEYS][Constants::OCTAVE] = 
{
    { N, F,  N,  F,  F,  N, F,  N, F,  N, F, F}, /* gb */
    { N, F,  N,  F,  F,  N, F,  N, F,  N, F, F}, /* db */
    { N, F,  N,  F,  F,  N, F,  N, F,  N, F, F}, /* ab */
    { N, F,  N,  F,  F,  N, F,  N, F,  N, F, F}, /* eb */
    { N, F,  N,  F,  F,  N, F,  N, F,  N, F, F}, /* bb */
    { N, F,  N,  F,  N,  N, F,  N, F,  N, F, F}, /* f  */
    { N, F,  N,  F,  N,  N, F,  N, F,  N, F, N}, /* c  */
    { N, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* g  */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* d  */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* a  */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* e  */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* b  */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}, /* f# */
    { S, S,  N,  S,  N,  S, S,  N, S,  N, S, N}  /* c# */
};

/**
 * newPitchesForNotes makes from a NSList a new NSList
 * with the same durations as in notes, but with pitches 
 * determined from NSList pitches.
 * If there are fewer NoteSymbols in pitches than there are in notes,
 * then pitches are used again from the beginning, and so on, until
 * all NoteSymbols in notes are used up.
 * @param notes
 * @param pitches
 * @return 
 */

/*static*/ NoteSymbol::NSList 
NoteSymbol::newPitchesForNotes(NSList &notes, NSList &pitches)
{
    if(pitches.size() == 0)
        return notes;

    NSList result;
    auto itP = pitches.begin();
    for(auto x : notes)
    {

        if(itP == pitches.end())
            itP = pitches.begin();

        if(x->isRest())
        {
            result.push_back(x);
        }
        else
        {
            auto y = *itP++;
            int dur = x->getDuration();
            if( y->isRest() )
              result.push_back(NoteSymbol::getRestSymbol(dur));
            else
              result.push_back(NoteSymbol::makeNoteSymbol(y->getMIDI(), dur));
        }
    }
    return result;
}