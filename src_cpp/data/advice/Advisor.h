#pragma once

#include "polya/Polylist.h"
#include "data/ChordForm.h"

#include <array>
#include <vector>
#include <unordered_map>

/**
 * Contains a set of rules read and processed during construction that can
 * be queried for Advice.
 */
class Advisor
{
public:
    Advisor();
    void SetRules(Polylist::Ptr p);

    enum class CellFlavor
    {
        k_Plain,
        k_Idiom,
        k_Rhythm
    };

    enum class LickFlavor
    {
        k_Plain,
        k_Quote,
        k_Brick
    };

private:
    bool addOneRule(Polylist::ObjPtr r, unsigned serial, bool marked, bool allowDups);
    bool addLick(Polylist* r, LickFlavor f, unsigned serial, bool marked,
                bool allowDups, LickFlavor otherFlavor);
    Polylist::Ptr addLickForm(Polylist *r, unsigned serial, bool marked, 
                Polylist::Ptr old, long grade, bool allowDups, 
                Polylist::Ptr otherFlavor);
    bool addCell(Polylist *r, CellFlavor f, unsigned serial, bool marked);
    bool addChord(Polylist *r);
    bool addScale(Polylist *r);

    char const *k_adviceTok;
    char const *k_approachTok;
    char const *k_brickTok;
    char const *k_rhythmTok;
    char const *k_cellTok;
    char const *k_chordTok;
    char const *k_chordsTok;
    char const *k_colorTok;
    char const *k_drumPatternTok;
    char const *k_extensionsTok;
    char const *k_familyTok;
    char const *k_gradeTok;
    char const *k_idiomTok;
    char const *k_keyTok;
    char const *k_lickTok;
    char const *k_markedTok;
    char const *k_nameTok;
    char const *k_notesTok;
    char const *k_priorityTok;
    char const *k_quoteTok;
    char const *k_sameTok;
    char const *k_scaleTok;
    char const *k_scalesTok;
    char const *k_sequenceTok;
    char const *k_serialTok;
    char const *k_spellTok;
    char const *k_styleTok;
    char const *k_substituteTok;
    char const *k_voicingsTok;
    char const *k_barTok;
    char const *k_slashTok;

private:
    Polylist::Ptr m_ruleArray; // arrives via SetRules
    std::unordered_map<char const *, Polylist::Ptr> m_scales;
    std::unordered_map<char const *, ChordForm::Ptr> m_chords;
    std::array<Polylist::Ptr, 2> m_cells; // indexed by cell flavor
    std::array<Polylist::Ptr, 3> m_licks; // indexed by lick flavor
    Polylist m_rhythms; 
    std::vector<Polylist::Ptr> m_markArray;

    /**
     * the table listing, for each chord, the chords it extends
     * (either directly, or indirectly)
     */
    Polylist m_extensionsTable;
    Polylist m_invertedExtensionsTable;

    unsigned m_serialNo; // used to number licks and cell advice items
    unsigned m_interChordTolerance; // half-beats, used in matching
    unsigned m_numChords;
    unsigned m_metre[2];
};