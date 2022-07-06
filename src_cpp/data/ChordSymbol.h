#pragma once
#include <string>
/**
 * A ChordSymbol is an object representing a chord in lightweight form,
 * from which a Chord can be created given a rhythmValue or duration and
 * possibly other parameters. 
 * It is like a pre-parsed string version of a chord from a leadsheet.
 * The parsed components are:
 *
 * The type of chord, such as M7, 7, 7#5, etc.
 * The root of the chord
 * The bass note (defined to be the root if not otherwise specified).
 * The polychord component, which is another ChordSymbol over which this chord is placed.
 */
class ChordSymbol
{

private:
    std::string m_name;
    std::string m_type;

};