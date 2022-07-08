#include "Key.h"
#include "util/String.h"
#include <cassert>

Key Key::keys[] =
{
    {0, "gb", 6},
    {1, "db", 11},
    {2, "ab", 4},
    {3, "eb", 9},
    {4, "bb", 2},
    {5, "f", 7},
    {6, "c", 0},
    {7, "g", 5},
    {8, "d", 10},
    {9, "a", 3},
    {10, "e", 8},
    {11, "b", 1},
    {12, "f#", 6},
    {13, "c#", 11},
};

Key &Key::Ckey = keys[6];
const int Key::SEMITONEOFFSET = 7;

/* scales corresponding to keys */
int Key::cycleIndices[][k_Octave] =
{
    {gb, db, ab, eb, bb, f, c, g, d, a, fb, cb},
    {db, ab, eb, bb, f, c, g, d, a, fb, cb, gb},
    {ab, eb, bb, f, c, g, d, a, fb, cb, gb, db},
    {eb, bb, f, c, g, d, a, fb, cb, gb, db, ab},
    {bb, f, c, g, d, a, fb, cb, gb, db, ab, eb},
    {f, c, g, d, a, e, cb, gb, db, ab, eb, bb},
    {c, g, d, a, e, b, gb, db, ab, eb, bb, f},
    {g, d, a, e, b, fs, cs, gs, ds, bb, es, c},
    {d, a, e, b, fs, cs, gs, ds, bb, es, bs, g},
    {a, e, b, fs, cs, gs, ds, as, es, bs, g, d},
    {e, b, fs, cs, gs, ds, as, es, bs, g, d, a},
    {b, fs, cs, gs, ds, as, es, bs, g, d, a, e},
    {fs, cs, gs, ds, as, es, bs, g, d, a, e, b},
    {cs, gs, ds, as, es, bs, g, d, a, e, b, fs}
};

  int Key::chromaticIndices[][k_Octave] = 
  {
    {c, db, d, eb, fb, f, gb, g, ab, a, bb, cb}, /* fb */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, cb}, /* cb */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, cb}, /* gb */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, cb}, /* db */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, b}, /* ab */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, b}, /* eb */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, b}, /* bb */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, b}, /* f  */
    {c, db, d, eb, e, f, gb, g, ab, a, bb, b}, /* c  */
    {c, db, d, eb, e, f, fs, g, ab, a, bb, b}, /* g  */
    {c, cs, d, eb, e, f, fs, g, ab, a, bb, b}, /* d  */
    {c, cs, d, ds, e, f, fs, g, gs, a, bb, b}, /* a  */
    {c, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* e  */
    {c, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* b  */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* f# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* c# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* g# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* d# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* a# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b}, /* e# */
    {bs, cs, d, ds, e, f, fs, g, gs, a, as, b} /* b# */
};

/**
 * table used to adjust input notes from staves, depending on key signature
 */
int Key::adjustPitchInKey[][k_Octave] =
{
    /*  c      d       e   f      g      a      b */
    {-1, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0, -1}, /* gb -6 */
    {0, 0, -1, 0, -1, 0, 0, -1, 0, -1, 0, -1}, /* db -5 */
    {0, 0, -1, 0, -1, 0, 0, 0, 0, -1, 0, -1}, /* ab -4 */
    {0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, -1}, /* eb -3 */
    {0, 0, 0, 0, -1, 0, 0, 0, 0, 0, 0, -1}, /* bb -2 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1}, /* f  -1 */
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* c   0 */
    {0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, /* g   1 */
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0}, /* d   2 */
    {1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0}, /* a   3 */
    {1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0}, /* e   4 */
    {1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0}, /* b   5 */
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 0}, /* f#  6 */
    {1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 0, 1} /* c#  7*/ 
    /*  c      d       e   f      g      a      b */
};

/** transpositions of one key to another */
int Key::transpositions[][k_Octave] =
{
    {gbkey, gkey, abkey, akey, bbkey, bkey, ckey, dbkey, dkey, ebkey, ekey, fkey},
    {dbkey, dkey, ebkey, ekey, fkey, gbkey, gkey, abkey, akey, bbkey, bkey, ckey},
    {abkey, akey, bbkey, bkey, ckey, dbkey, dkey, ebkey, ekey, fkey, gbkey, gkey},
    {ebkey, ekey, fkey, gbkey, gkey, abkey, akey, bbkey, bkey, ckey, dbkey, dkey},
    {bbkey, bkey, ckey, dbkey, dkey, ebkey, ekey, fkey, gbkey, gkey, abkey, akey},
    {fkey, gbkey, gkey, abkey, akey, bbkey, bkey, ckey, dbkey, dkey, ebkey, ekey},
    {ckey, dbkey, dkey, ebkey, ekey, fkey, gbkey, gkey, abkey, akey, bbkey, bkey},
    {gkey, abkey, akey, bbkey, bkey, ckey, cskey, dkey, ebkey, ekey, fkey, fskey},
    {dkey, ebkey, ekey, fkey, fskey, gkey, abkey, akey, bbkey, bkey, ckey, cskey},
    {akey, bbkey, bkey, ckey, cskey, dkey, ebkey, ekey, fkey, fskey, gkey, abkey},
    {ekey, fkey, fskey, gkey, abkey, akey, bbkey, bkey, ckey, cskey, dkey, ebkey},
    {bkey, ckey, cskey, dkey, ebkey, ekey, fkey, fskey, gkey, abkey, akey, bbkey},
    {fskey, gkey, abkey, akey, bbkey, bkey, ckey, cskey, dkey, ebkey, ekey, fkey},
    {cskey, dkey, ebkey, ekey, fkey, fskey, gkey, abkey, akey, bbkey, bkey, ckey}
};

/* ------------------------------------------------------------------------ */
Key::Key(int index, char const *nm, int cpos)
{
    m_index = index;
    m_name = PListSymbol::getSymbol(nm);
    m_cPosition = cpos; // position of c in this key
}

PitchClass const &
Key::renderInKey(std::string const &name) const
{
    int position = PitchClass::getPitchClass(name).getIndex();
    if(position < 0)
      return PitchClass::sInvalid;

    int pitchIndex = (keys[m_index].m_cPosition + position) % k_Octave;
    return PitchClass::getPitchClass(chromaticIndices[m_index][pitchIndex]);
}

PitchClass const &
Key::rep(int offset) const
{
    if(offset < 0)
      offset += (-offset / k_Octave + 1) * k_Octave;
    offset = offset % k_Octave;
    return PitchClass::getPitchClass(chromaticIndices[m_index][offset]);
}

Key const *
Key::transpose(int semis) const
{
    if(semis < 0)
      semis = k_Octave - (-semis) % k_Octave;
    assert(semis >= 0);
    semis = semis % k_Octave;
    int newIndex = transpositions[m_index][semis];
    assert (newIndex >= 0 && newIndex < KEY_SIZE);
    return &keys[newIndex];
}

std::string 
Key::toString() { return m_name; }

/* - class methods -------------------------------------------------------- */
/*static*/ Key const *
Key::getKey(std::string const &keynm)
{
    std::string lkey = UtString::toLower(keynm);
    return getKey(PListSymbol::getSymbol(lkey));
}

/*static*/ Key const *
Key::getKey(char const *tok)
{
    for(int i=0;i<=13;i++)
    {
        if(keys[i].m_name == tok)
            return &keys[i];
    }
    return nullptr;
}

/*static*/ char const *
Key::getKeyName(int index)
{
    if(index <= 13)
        return keys[index].m_name;
    else
        return nullptr;
}

int Key::getKeyDelta(int sharps, int rise)
{
    int newSharps = sharps + rise * SEMITONEOFFSET;
    while(newSharps < Constants::MIN_KEY)
      newSharps += k_Octave;

    while(newSharps > Constants::MAX_KEY)
      newSharps -= k_Octave;

    return newSharps;
}

PitchClass const &
Key::transpose(PitchClass pc, int semitones)
{
    return pc.transpose(semitones);
}

Polylist Key::transposeChordList(Polylist &chordSeq, int rise)
{
    return transposeChordList(chordSeq, rise, Ckey);
}
Polylist Key::transposeChordList(Polylist &chordSeq, int rise, Key const &key)
{
    if(rise == 0 || chordSeq.isEmpty())
      return chordSeq;

    // make a new list of the transposed chords in the render
    Polylist newChords;
    #if 0
    while(chordSeq.nonEmpty())
    {
        String item = (String)chordSeq.first();
        // For now, we are skipping bar-line info
        if(!Advisor::licksIgnore.member(item))
            newChords = newChords.cons(transposeChord(item, rise, key));
        chordSeq = chordSeq.rest();
    }
    return newChords.reverse();
    #else
    assert(0);
    return newChords;
    #endif
}

std::string 
Key::transposeChord(std::string const &chord, int rise, Key const &key)
{
    if(rise == 0 || chord == Constants::NOCHORD)
      return chord;

    using OPtr = Polylist::ObjPtr;
    Polylist exploded = explodeChord(chord);
    if(exploded.size() == 0)
        return "";

    OPtr a = exploded.first();
    OPtr b = exploded.second();
    OPtr c = exploded.third();

    std::string root = a->asString();
    std::string body = b ? b->asString() : "";
    std::string afterSlash = c ? c->asString() : "";

    PitchClass const & pc = PitchClass::getPitchClass(root);

    assert(pc.isValid());

    PitchClass newRoot = pc.transpose(rise);

    if(afterSlash.size() == 0)
        return newRoot.getChordBase() + body;

    // Deal with slash-chord
    OPtr d = exploded.fourth();
    PitchClass bass = PitchClass::getPitchClass(d->asString());
    assert(bass.isValid());
    std::string newBass = bass.transpose(rise).getChordBase();
    return newRoot.getChordBase() + body + "/" + newBass;
}

std::string Key::makeCroot(std::string const &chord)
{
    return makeRoot(Constants::CROOT, chord);
}

std::string Key::makeRoot(std::string const &root, std::string const &chord)
{
    Polylist exploded = explodeChord(chord);
    if(exploded.isValid())
    {
        std::string body = exploded.second()->asString();
        std::string afterSlash = exploded.third()->asString();
        if(afterSlash.size() == 0)
            return root + body;

        std::string origRoot = exploded.first()->asString();
        int rise = PitchClass::findRise(UtString::toLower(root), origRoot);
        // Deal with slash-chord
        std::string bstr = exploded.fourth()->asString();
        PitchClass const &bass = PitchClass::getPitchClass(bstr);
        assert(bass.isValid());
        std::string newBass = bass.transpose(rise).getChordBase();
        return root + body + "/" + newBass;
    }
    else
        return "";
}

std::string Key::getRoot(std::string const &chord)
{
    return explodeChord(chord).first()->asString();
}

bool Key::sameRoot(std::string const &chord1, std::string const &chord2)
{
    return getRoot(chord1) == getRoot(chord2);
}

bool Key::isValidStem(std::string const &stem)
{
    if(stem.size() == 0) return false;
    switch(stem[0])
    {
    case 'A':
    case 'B':
    case 'C':
    case 'D':
    case 'E':
    case 'F':
    case 'G':
        return true;
    default:
        return false;
    }
}

bool Key::hasValidStem(std::string const &chord)
{
    return explodeChord(chord).isValid();
}

Polylist Key::explodeChord(std::string const &chord)
{
    size_t len = chord.size();
    if(len > 0)
    {
        std::string buffer1;
        buffer1.push_back(chord[0]);
        if(isValidStem(buffer1))
        {
            int index = 1;
            if(index < len)
            {
                char c = chord[1];
                if(c == Constants::SHARP || c == Constants::FLAT)
                {
                    buffer1.push_back(c);
                    index++;
                }
            }
            std::string root = buffer1;
            std::string rootLower = UtString::toLower(root);
            if(PitchClass::isValidPitch(rootLower))
            {
                // Get the type of the chord.
                std::string buffer2;
                while(index < len && chord[index] != Constants::SLASH)
                {
                    buffer2.push_back(chord[index]);
                    index++;
                }

                std::string body = buffer2;
                std::string bass = rootLower;
                std::string afterSlash;
                if(index < len)
                {
                    // We have a slash chord.
                    index++;	// skip the slash

                    std::string buffer3;
                    while(index < len)
                    {
                        buffer3.push_back(chord[index]);
                        index++;
                    }
                    afterSlash = buffer3;
                    std::string slashLower = UtString::toLower(afterSlash);
                    if(!PitchClass::isValidPitch(slashLower))
                        return Polylist();
                    else
                        bass = slashLower;
                }
                std::vector<std::string> x = {root, body, afterSlash, bass};
                return Polylist::Polylist(x);
            }
        }
    }
    return Polylist();
}

// here we expect L to be a list of string/value pairs
Polylist Key::invalidNotes(Polylist &L)
{
    Polylist ret;
    if(L.size() > 0)
    {
        using OPtr = Polylist::ObjPtr;
        using OListIt = Polylist::ObjListIt;
        OListIt a = L.getBegin();
        OListIt b = L.getEnd();
        while(a != b)
        {
            OPtr o = *a;
            if(o->getType() == Polylist::k_list)
            {
                OPtr f = L.first();
            } 
            a++;
        }
    }
    return ret;
}

int Key::pitchFromLeadsheet(std::string const &str, int rise)
{
    return 0;
}

Note Key::noteFromLeadsheet(std::string const &str, int rise, int slotsPerBeat)
{
    return Note();
}

Note Key::noteFromLeadsheet(std::string const &str, int rise, int slotsPerBeat,
                            Key const &)
{
    return Note();
}

Polylist Key::transposeNoteStringList(Polylist &L, int rise, 
                                        Key const &key)
{
    return Polylist();
}

std::string Key::transposeNoteString(std::string const &noteString, int rise, 
                                        Key const &key)
{ 
    return std::string();
}

Polylist Key::transposeNoteStringListToNumbers(Polylist &L, int rise,
                                                Key const &key)
{
    return Polylist();
}
std::string Key::transposeNoteStringToNumbers(std::string const &noteString,
                                                int rise, Key const &key)
{
    return std::string();
}
std::string Key::transposeOne(std::string const &from, std::string const &to, 
                            std::string const &p, Key const &key)
{
    return std::string();
}

Polylist Key::transposeList(std::string const &from, std::string const &to, 
                        Polylist &L, Key const &key)
{
    return Polylist();
}
bool Key::enharmonic(std::string const &x, std::string const &y)
{
    return false;
}
bool Key::enhMember(std::string const &x, Polylist &L)
{
    return false;
}
bool Key::test(std::string const &name)
{
    return false;
}

Note Key::makeNote(std::string const &pitchClassName, int midiBase,
                            int duration)
{
    return Note();
}
Note Key::makeNoteAbove(std::string const &pitchClassName, int midiBase,
                        int minimum, int duration)
{
    return Note();
}
