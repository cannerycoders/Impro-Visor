#include "Part.h"
#include "MelodyPart.h"
#include "ChordPart.h"
#include "Note.h"
#include "Chord.h"

/* ------------------------------------------------------------------- */

int const Part::DEFAULT_INSTRUMENT = 0;
int const Part::DEFAULT_VOLUME = 85;
int const Part::DEFAULT_SIZE = 0;
float Part::DEFAULT_SWING = .67f;
int Part::DEFAULT_KEYSIG = 0;

/* ------------------------------------------------------------------- */

/* ------------------------------------------------------------------- */
Part::Part(int size)
{
    // m_title, m_composer default to ""
    m_volume = DEFAULT_VOLUME;
    m_instrument = DEFAULT_INSTRUMENT;
    m_staveType = Preferences::get().getStaveType();
    setMetre(Constants::DEFAULT_METRE[0], Constants::DEFAULT_METRE[1]);
    m_keySig = DEFAULT_KEYSIG;
    m_swing = DEFAULT_SWING;
    if(size != 0)
    {
        // empty part has one long rest
        m_slots.resize(size); // fills with size std::unique_ptr(nullptr)
        m_slots[0] = std::unique_ptr<IUnit>(Note::MakeRest(size)); 
        m_unitCount = 1;
    }
}

/* currently we anticipate that the default copy-constructor works */

void
Part::setMetre(int top, int bottom)
{
    m_metre[0] = top;
    m_metre[1] = bottom;
    m_beatValue = Constants::WHOLE / m_metre[1];
    m_measureLength = m_beatValue * m_metre[0];
}

int
Part::getBars()
{ 
    return (int) std::ceilf(m_slots.size()/(float) m_measureLength);
}

int 
Part::getActiveBars()
{
    int lastActiveSlot = this->getLastActiveSlot();
    if(lastActiveSlot < 0)
        return 0;

    int activeBars = 1 + (int) std::ceilf((float) lastActiveSlot / m_measureLength);
    return activeBars;
}

/**
 * Returns the last active slot, if any. If none is active, then return -1.
 */
int
Part::getLastActiveSlot()
{
    for(size_t j = m_slots.size() - 1; j >= 0; j--)
    {
        UnitPtr o = m_slots[j];
        if(o && o->isActive()) return (int) j;
    }
    return -1;
}

/**
 * @brief get that subset of slots that contain valid units.
 * 
 * @return Part::t_UnitList 
 */
Part::t_UnitList
Part::getUnitList()
{
    t_UnitList ret;
    partIterator it(m_slots);
    while(it.hasNext())
        ret.push_back(it.next());
    return ret;
}

/**
 * Returns the Unit at the specified slotIndex
 * @param slotIndex     the index of the Unit to get
 * @return Unit         the Unit at the specified index
 */
Part::UnitPtr 
Part::getUnit(int slotIndex) 
{
    if(slotIndex < 0 || slotIndex >= m_slots.size())
        return UnitPtr();
    else
        return m_slots[slotIndex];
}

void
Part::setSize(int sz)
{
    if(sz == m_slots.size()) return;

    m_slots.resize(sz);

    int lastUnitIndex = -1;
    for(int i=0;i<m_slots.size();i++)
    {
        if(m_slots[i])
            lastUnitIndex = i;
    }

    // If there is a last unit, its rhythmValue may need lengthening or
    // shortening to conform to our new new part length.
    if(lastUnitIndex >= 0)
        m_slots[lastUnitIndex]->setRhythmValue(sz - lastUnitIndex);
}

Part::UnitPtr 
Part::getPrevUnit(int slotIndex)
{
    int i = getPrevIndex(m_slots, slotIndex);
    if(i == -1)
        return UnitPtr();
    else
        return m_slots[i];
}

/*static*/ int
Part::getNextIndex(t_UnitList const &slots, int si)
{
    for(int i = si + 1; i < slots.size(); i++)
    {
        UnitPtr unit = slots[i];
        if(unit)
           return i;
    }
    return (int) slots.size();
}

/*static*/ int
Part::getPrevIndex(t_UnitList const &slots, int si)
{
    if(si < 0)
        return -1;
    
    partIterator i(slots, si);
    while(i.hasPrevious())
    {
        UnitPtr unit = i.previous();
        if(unit)
            return i.nextIndex;
    }
    return -1;
}

std::string 
Part::toString()
{
    std::string result;
    result.reserve(1000);
    result.append("\nPart with unitCount = ");
    result.append(std::to_string(m_unitCount));
    result.append(": ");
    partIterator it(m_slots);
    while(it.hasNext())
    {
        int index = it.nextIndex;
        result.append("Beat ");
        result.append(std::to_string(index / m_beatValue));
        result.append(" + ");
        result.append(std::to_string(index % m_beatValue));
        result.append(" slots :");
        result.append(it.next()->toString());
        result.append("\n");
    }
    return result;
}

/**
 * Adds a Unit to the end of the Part, extending the length as it goes. Note
 * that 0 duration units should not be added, as they will cause this to fail.
 *
 * @param unit the Unit to add
 */
void
Part::addUnit(UnitPtr unit)
{
    if(!unit) return;

    int rv = unit->getRhythmValue();
    if( rv <= 0 )
    {
        // Not sure how a unit can have 0 as its rhythm value, but it does 
        // happen, and it causes problems.
        return;
    }

    //Trace.log(0, "adding unit to end of part " + unit.toLeadsheet() + ", rv = " + rv + ", slots.size() = " + slots.size());

    size_t index = m_slots.size();
    size_t newSize = m_slots.size() + rv;
    m_slots.resize(newSize); // fills with shared_ptr(null)

     //Trace.log(0, "now slots.size() = " + slots.size() + ", index = " + index);
    m_unitCount++;

    m_slots[index] = unit;

    //rk Hack to remove accidental if key signature covers it.
    // This tends to reduce the number of accidentals in the notation.
    auto note = std::dynamic_pointer_cast<Note>(unit);
    if(note)
    {
        if(note->isAccidentalInKey(m_keySig))
        {
            // If the note shows as accidentaly in the key,
            // see if toggling it will help, and if not, 
            // toggle back.
            note->toggleEnharmonic();
            if(note->isAccidentalInKey(m_keySig))
                note->toggleEnharmonic();
        }
    }
}

/**
 * Sets the slot at the specified unitIndex to the specified Unit.
 *
 * @param unitIndex the index of the slot to put the Unit in
 * @param unit the Unit to set
 */
void 
Part::setUnit(int unitIndex, UnitPtr unit)
{
    //System.out.println("setUnit " + unitIndex + " to " + unit);
    if(unit && unitIndex >= m_slots.size() || unitIndex < 0)
    {
        return; // shouldn't happen, but can.
    }

    //Trace.log(0, "setting Unit at " + unitIndex + " to " + (unit == null ? null : unit.toLeadsheet()));
    // if we want to set it to empty, we are effectively deleting it
    if(!unit)
    {
        delUnit(unitIndex);	// Tracing this produces too much output
        return;
    }

    //Trace.log(3, "setting Unit at " + unitIndex + " to " + unit.toLeadsheet());

    //rk: I really do not follow the logic having to do with old note values.

    UnitPtr oldUnit = m_slots[unitIndex];

    int rv = getUnitRhythmValue(unitIndex);

    // if the slot is empty, we need to find what the rhythm value should be
    if(!oldUnit)
    {
        // Note: When next unit is a rest, the above may had the effect of 
        // cutting the inserted note short!! See compensating code below.
        int nextIndex = getNextIndex(m_slots, unitIndex);
        m_unitCount++;
        UnitPtr prevUnit = getPrevUnit(unitIndex);
        if(prevUnit)
        {
            //Trace.log(3, "in setUnit - A, setting rhythmValue");
            // we also need to change the rv of the previous Unit
            prevUnit->setRhythmValue(prevUnit->getRhythmValue() - rv);
        }
    }
    else
    {
        // if there was already a Unit there, we already know the rv
        rv = oldUnit->getRhythmValue();
    }

    //Trace.log(3, "in setUnit - B, setting rhythmValue");
    unit->setRhythmValue(rv);
    m_slots[unitIndex] = unit;
}

void 
Part::newSetUnit(int unitIndex, UnitPtr unit)
{
    //checkConsistency("start of newSetUnit");
    if(unitIndex >= m_slots.size() || unitIndex < 0 )
    {
        return; // shouldn't happen, but can?
    }

    if(!unit)
    {
        delUnit(unitIndex);
        return;
    }

    // Pre-conditioning: If unit is too long for part, truncate it first:
    int unitDuration = unit->getRhythmValue();
    int nextUnitStart = unitIndex + unitDuration;
    if(nextUnitStart >= m_slots.size())
    {
        unit = UnitPtr(unit->copy());
        unit->setRhythmValue((int) (m_slots.size() - unitIndex));
        nextUnitStart = (int) m_slots.size();
    }

    // If this unit overlays one or more units, set them to null.
    // Let makeConsistent fix things up.
    
    for(int index = unitIndex; index < nextUnitStart; index++)
    {
        m_slots[index] = UnitPtr();
    }
    
    // If the new unit overlays part of another unit, replace the latter
    // with a rest.
    if(nextUnitStart < m_slots.size() && !m_slots[nextUnitStart])
    {
        int nextUnitEnd = getNextIndex(m_slots, nextUnitStart);
        if(dynamic_cast<MelodyPart *>(this))
        {
            m_slots[nextUnitStart] = UnitPtr(Note::MakeRest(nextUnitEnd - nextUnitStart));
        }
        else
        {
          // HB - Disabled as of 7-8-13, not sure this is needed when pasting
          //       chords back into leadsheet.
          // slots.set(nextUnitStart, new Chord(nextUnitEnd - nextUnitStart));            
        }
    }

    // Place the new unit
    setSlot(unitIndex, unit, "leaving newSetUnit");
    
    // Re-enable this!! checkConsistency("end of newSetUnit");
}

void 
Part::setSlot(int index, UnitPtr unit, char const *msg)
{
    //System.out.println("\nsetting slot at " + message + " index " + index + " to " + unit);
    if(!unit) return;

    m_slots[index] = unit;
    
    // need to account for implicitly deleted last unit

    makeConsistent();
    //System.out.println("part after setting slot:  " + this);
}

void 
Part::makeConsistent()
{
    if(m_slots.size() == 0)
    {
        m_unitCount = 0;
        return;
    }

    //checkConsistency("start makeConsistent");
    int prevIndex = 0;
    UnitPtr prevUnit = m_slots[prevIndex];
    UnitPtr thisUnit;

    if(!prevUnit)
    {
        m_slots[prevIndex] = UnitPtr(Note::MakeRest(60));
    }

    int count = 1;
    int index = 1;
    for( ; index < m_slots.size() ; index++)
    {
        thisUnit = m_slots[index];
        if(thisUnit)
        {
            // Found another unit
            count++;
            int diff = index - prevIndex;
            if(isRest(thisUnit) && isRest(prevUnit))
            {
                // merge adjacent rests
                count--;
                prevUnit->setRhythmValue(prevUnit->getRhythmValue() + thisUnit->getRhythmValue());
                m_slots[index] = UnitPtr();
            }
            else 
            if(diff != prevUnit->getRhythmValue() )
            {
                // Adjust prevUnit to fill gap
                prevUnit->setRhythmValue(diff);
            }
            prevUnit = thisUnit;
            prevIndex = index;
        }
    }

    prevUnit->setRhythmValue(index - prevIndex);
    m_unitCount = count;
    //System.out.println("makeConsistent result " + this);
    // re-enable this checkConsistency("end makeConsistent");
}

/**
 * Returns the rhythm value of a unit starting at the unitIndex. This function
 * can be called on an empty slot to see what rhythm value a Unit would have if
 * it was in that slot.
 *
 * @param unitIndex the index of the Unit in question
 * @return int the rhythm value of the Unit
 */
int 
Part::getUnitRhythmValue(int unitIndex)
{
    //System.out.print("getUnitRhythmValue " + unitIndex + " in part of length " + size);
    if(unitIndex >= m_slots.size() )
        return 0;

    // Start on the next Unit over, since we can call this on an 
    // empty slot
    int i = unitIndex + 1;
    
    while(i < m_slots.size())
    {
        if(getUnit(i))
        {
            //System.out.println(" " + (i - unitIndex));
            return i - unitIndex;
        }
        i++;
    }
    //System.out.println(" " + (size - unitIndex));
    return (int) (m_slots.size() - unitIndex);
}

void 
Part::delUnit(int unitIndex)
{
    if(unitIndex < 0 || unitIndex >= m_slots.size()) return;

    UnitPtr unit = m_slots[unitIndex];

    if(!unit)
    {
        //Trace.log(0, "delUnit at " + unitIndex + ", was " + unit);
        int rv = unit->getRhythmValue();
        m_slots[unitIndex] = UnitPtr();
        m_unitCount--;
        UnitPtr prevUnit = getPrevUnit(unitIndex);

        //Trace.log(3, "prevUnit = " + prevUnit);

        // If there was a Unit before it, we need to adjust its rv.

        if(!prevUnit)
        {
            //Trace.log(0, "in delUnit, setting rhythmValue");
            prevUnit->setRhythmValue(prevUnit->getRhythmValue() + rv);
        }
        // If there was no Unit before it, then we just deleted the
        // 0 slot, which must never be empty, so put something appropriate there.
        else 
        if(dynamic_cast<MelodyPart *>(this))
        {
            setUnit(0, UnitPtr(Note::MakeRest()));
        }
        else 
        if(dynamic_cast<ChordPart *>(this))
        {
            setUnit(0, UnitPtr(new Chord(Constants::NOCHORD)));
        }
    }
}

/* -------------------------------------------------------------------------- */
Part::partIterator::partIterator(t_UnitList const &unitList, int ni) :
    slots(unitList)
{
    this->unitIndex = 0;
    this->unitCount = 0;
    for(int i=0;i<unitList.size();i++)
    {
        if(this->slots[i])
            this->unitCount++;
    }
    if(ni != 0)
    {
        int i=0;
        while(i < ni)
        {
            if(slots[i].get())
                this->unitIndex++;
            i++;
        }
    }
    this->nextIndex = ni;
    this->prevIndex = ni-1;
}

bool 
Part::partIterator::hasNext()
{
    return this->unitIndex < this->unitCount && 
            this->nextIndex < slots.size();
}
bool 
Part::partIterator::hasPrevious()
{
    return this->unitIndex  > 0;
}

Part::UnitPtr 
Part::partIterator::next()
{
    if(hasNext())
    {
        this->prevIndex = this->nextIndex;
        UnitPtr unit = slots[nextIndex];
        this->nextIndex = getNextIndex(slots, this->nextIndex);
        this->unitIndex++;
        return unit;
    }
    return UnitPtr();
}

Part::UnitPtr 
Part::partIterator::previous() 
{
    if(hasPrevious()) 
    {
        this->nextIndex = this->prevIndex;
        UnitPtr unit = slots[nextIndex];
        prevIndex = getPrevIndex(slots, prevIndex);
        unitIndex--;
        return unit;
    }
    return UnitPtr();
}