#pragma once

#include "Constants.h"
#include "polya/Polylist.h"
#include "Key.h"
#include <string>
#include <array>
#include <iostream>

/**
 * A ChordForm houses the form information for a chord.
 * The actual chord information is determined by supplying a root pitch.
 **/
class ChordForm
{
private:
    /**
     * The constructor is private. A factory is used so that
     * error-checking can be done prior to construction
     */
    int PRIORITY_PREFIX_MAX = 5; 
    std::string m_name;
    std::string m_same;
    Key const *m_key;
    std::string m_family;
    Polylist m_pronounce;
    Polylist m_spell;
    Polylist m_priority;
    Polylist m_avoid;
    Polylist m_color;
    Polylist m_approach;
    Polylist m_scales;
    Polylist m_voicings;
    Polylist m_extensions;
    Polylist m_substitutions;
    ChordForm();

public:
    static char const * APPROACH;
    static char const * AVOID;
    static char const * COLOR;
    static char const * EXTENSION;
    static char const * EXTENSIONS;
    static char const * FAMILY;
    static char const * KEY;
    static char const * NAME;
    static char const * NOTES;
    static char const * PRIORITY;
    static char const * PRONOUNCE;
    static char const * SAME;
    static char const * SCALES;
    static char const * SPELL;
    static char const * SUBS;
    static char const * TYPE;
    static char const * DEFAULT_FAMILY;
    static char const * VOICINGS;
    static char const * VOICING_WILD_CARD;

    using Ptr = std::shared_ptr<ChordForm>;
    static Ptr makeChordForm(Polylist &arg);
    static Polylist keepPositiveProbs(Polylist &L);

private:
    static std::string padTo(std::string &in, int desiredLen, char padChar);

public:
    /**
     * Add a new voicing
    @param voicing
    */
    void addVoicing(std::string& id, std::string& type, 
                    Polylist &notes, Polylist &extension);

    /**
     * Remove first voicing with the given id
     * @param voicing
    */
    void removeVoicing(std::string &id);

    /**
     * Remove the voicing in the given position
     *  @param voicing
     */

    void removeNthVoicing(int position);

    void showForm(std::ostream &);
    void showName(std::ostream &);

    std::string const & getName() { return m_name; }

    /**
     * Returns the name of the chord for which this chord is a synonym, if any; otherwise null 
     * @return the name of the chord for which this chord is a synonym, if any; otherwise null 
     */
    std::string const & getSame() { return m_same; }

    /**
     * @return true if this chord is a synonym for another chord,
     * as indicated by a non-null 'same' field
     */
    bool isSynonym() { return m_same.size() != 0; }

    /**
     * Returns the family to which this chord belongs, if any is specified. Otherwise returns null.
     * @return the family to which this chord belongs
    */
    std::string const & getFamily();

    /**
     * Returns the color tones for this chord, relative to root "C"
     * @return the color tones for this chord, relative to root "C"
     */

    Polylist getColor() { return m_color; }

    /**
     * Returns the color tones for this chord, relative to the specified root
     * @return the color tones for this chord, relative to the specified root
    */
    Polylist getColor(std::string &root);
    Polylist getColor(std::string &root, Key key);

    Key const *getKey(std::string &root);
    Key const *getKey() { return m_key; }
    std::string const &getPrononunce();

    Polylist toPolylist();

    std::string toString() const;
    Polylist getSpell(std::string &root);
    Polylist getSpell(std::string &root, Key key);
    std::array<bool, Constants::OCTAVE> getSpellVector(std::string &root, Key key);
    std::vector<int> getSpellMIDIarray(std::string &root);
    std::vector<int> getScaleMIDIarray(std::string &root);
    std::vector<int> getColorMIDIarray(std::string &root);
    Polylist getPriority(std::string &root);
    Polylist getPriority(std::string &root, Key key);
    Polylist getScales();
    Polylist getFirstScale();
    Polylist getFirstScaleTones(std::string root);
    Polylist getPriority();
    Polylist getApproach();
    Polylist getApproach(std::string &root);
    Polylist getSubstitutions();
    Polylist getExtensions();

    /**
     * Add more color tones, such as ones implied by extensions.
    */
    void addColor(Polylist &newColor);
    Polylist generateVoicings(std::string &root, Key key);

    /**
     * Put voicings in a form for output
     * @param root
     * @return
     */
    Polylist getVoicingInfo(std::string &root);
    Polylist getVoicings(std::string&root, Key key);
    Polylist getVoicings(std::string &root, Key key, std::string &type);

    int getTypeIndex(Note &note, int transpose);

    /**
     * Test whether a list of NoteSymbols contains a specified interval
     * @param interval
     * @param notes
     * @return
    */
    static bool containsInterval(int interval, Polylist &notes);
    static Polylist permutations(Polylist &L);
    static bool isListOfStrings(Polylist &L);

};