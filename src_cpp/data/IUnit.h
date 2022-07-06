#pragma once

#include "Constants.h"

/**
 * IUnit can be thought of as anything that has a rhythm value, whether it
 * be a chord, a note, or a rest.
 * Since the code in Part for calculating rhythm value is not trivial, it makes
 * sense to share the code for everything that is put in a slot and has a 
 * rhythm value.  Notes and chords store pitch(es) differently,
 * play back differently, and are drawn differently, so this interface
 * is very useful.
 */
class IUnit
{
public:
    /**
     * the default rhythm value for a Unit
     */
    static const int DEFAULT_RHYTHM_VALUE;

    /**
     * Sets the Unit's rhythm value.
     * @param rhythmValue  an integer representing the Unit's rhythm value
     */
    enum class UnitType
    {
        k_Invalid,
        k_Note,
        k_Rest, /* a kind of note */
        k_Chord
    };
    virtual void setRhythmValue(int) = 0;
    virtual int getRhythmValue() const = 0;
    virtual IUnit *copy() const = 0;
    virtual bool isActive() const { return true; } // override for rest in Note
    virtual std::string toString() const = 0;
    virtual UnitType getUnitType() const { return UnitType::k_Invalid; }

    #if 0
     /**
     * Returns a String representation of the Unit
     * @return String   a String representation of the Unit
     */
    public String toString();
    public String toLeadsheet();
    public void save(BufferedWriter out) throws IOException;
    public void saveLeadsheet(BufferedWriter out, int[] metre) throws IOException;
    public void saveLeadsheet(BufferedWriter out, int[] metre, boolean lineBreaks) 
                throws IOException;
    #endif
};