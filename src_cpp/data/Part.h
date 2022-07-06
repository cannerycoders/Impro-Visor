#pragma once

#include "IUnit.h"
#include "Constants.h"
#include "util/Preferences.h"

#include <vector>

/**
 * Part represents an arbitrarily long melody or chord part, played by 
 * a specific instrument. A Part contains a sequence of Units stored 
 * in the vector<Unit> slots. It also contains information about the Part, 
 * such as volume, instrument, etc.  Units should be added using the setUnit 
 * method, which will automatically adjust rhythm values.  In a Part, the 
 * 0 slot must never be empty.  In its default state it contains a Rest or NC.  
 * If delUnit is called on the 0 slot, then it is replaced with a Rest or NC.
 */
class Part  // subclassed by MelodyPart and ChordPart
{
public:
    static int const DEFAULT_INSTRUMENT;
    static int const DEFAULT_VOLUME;
    static int const DEFAULT_SIZE; // default number of "slots"
    static int DEFAULT_KEYSIG;
    static float DEFAULT_SWING;

    using UnitPtr = std::shared_ptr<IUnit>;
    using t_UnitList = std::vector<UnitPtr>;

public:
    Part(int size);
    Part() : Part(DEFAULT_SIZE) {};
    virtual ~Part() {}

    void setTitle(std::string &t) { m_title = t; }
    std::string const &getTitle() { return m_title; }
    void setComposer(std::string &c) { m_composer = c; }
    std::string const &getComposer() { return m_composer; }
    Constants::StaveType getPreferredStaveType() { return m_staveType; }
    Constants::StaveType getStaveType() { return m_staveType; }
    void setStaveType(Constants::StaveType t) { m_staveType = t; }
    void setInstrument(int i) { m_instrument = i; }
    int getInstrument() { return m_instrument; }
    void setVolume(int v) { m_volume = v; }
    int getVolume() { return m_volume; }
    int getMeasureLength() { return m_measureLength; }
    void setMetre(int top, int bottom);
    int const *getMetre() { return m_metre; }
    void setKeySignature(int k) { m_keySig = k; }
    int getKeySignature() { return m_keySig; }
    float getSwing() { return m_swing; }
    void setSwing(float x) { m_swing = x; }
    size_t getSize() { return m_slots.size(); }
    size_t getEndTime() { return m_slots.size(); }
    size_t getNumBeats() { return m_slots.size() / m_beatValue; }
    int getBars();
    int getActiveBars();
    int getLastActiveSlot();
    t_UnitList getUnitList(); // subset of slots that have units
    void setSize(int newSize);
    std::string toString();
    void addUnit(UnitPtr u);
    void setUnit(int unitIndex, UnitPtr unit);
    UnitPtr getUnit(int slotIndex);
    void newSetUnit(int unitIndex, UnitPtr unit);
    void delUnit(int unitIndex);
    int getUnitRhythmValue(int unitIndex);

private:
    void setSlot(int index, UnitPtr unit, char const *msg);
    bool isRest(UnitPtr p) { return !p->isActive(); }
    void makeConsistent();

protected: //shared by MelodyPart et al
    std::string m_title;
    std::string m_composer;
    int m_instrument;
    int m_volume;
    int m_keySig;
    int m_metre[2];
    int m_beatValue;
    int m_measureLength;
    float m_swing;
    Constants::StaveType m_staveType;

    t_UnitList m_slots; // notes, rests, chords, etc.
    int m_unitCount; // number of slots that contain a Unit

    /**
     * Returns the index of the next Unit after the indicated slot.
     * Static since it's used in interator and Part.
     *
     * @param slotIndex the index of the slot to start searching from
     * @return int the index of the next Unit
     */
    static int getNextIndex(t_UnitList const &m_slots, int slotIndex);
    static int getPrevIndex(t_UnitList const &m_slots, int slotIndex);

    UnitPtr getPrevUnit(int slotIndex);

protected:
    struct partIterator
    {
        /**
         * Creates a new PartIterator that iterates over non-null entries
         * in the specified slots.
         * @param slots  a Vector<Unit> containing Units to iterate over
         * @param ni     the index of the first Unit to point to
         */
        partIterator(t_UnitList const &unitList, int ni=0);
        bool hasNext();
        bool hasPrevious();
        UnitPtr next();
        UnitPtr previous();

        t_UnitList const &slots;
        int unitIndex;
        int nextIndex;
        int prevIndex;
        int unitCount;

    }; // end partIterator def

    /**
     * @brief  create a part-iterator for our slots.
     * 
     * @param index 
     * @return partIterator& 
     */
    partIterator &iterator(int index)
    {
        return partIterator(m_slots, index);
    }

};