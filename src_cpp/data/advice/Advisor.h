#pragma once

#include "polya/Polylist.h"
#include <array>
#include <vector>

/**
 * Contains a set of rules read and processed during construction that can
 * be queried for Advice.
 */
class Advisor
{
public:
    using OListIt = Polylist::OListIt;
    using ListPtr = Polylist::ListPtr;
    using List = Polylist::list;
    using SymbolPtr = Polylist::SymbolPtr;
    using Symbol = Polylist::symbol;
    using StringPtr = Polylist::StringPtr;
    using String = Polylist::string;
    using LongPtr = Polylist::LongPtr;
    using Long = Polylist::integer;
    using NumberPtr = Polylist::NumberPtr;
    using Number = Polylist::number;

    Advisor();
    void SetRules(ListPtr);

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
    void addRules(ListPtr);
    bool addOneRule(ListPtr r, unsigned serial, bool marked, bool allowDups);
    bool addLick(ListPtr r, LickFlavor f, unsigned serial, bool marked,
                bool allowDups, LickFlavor otherFlavor);
    ListPtr addLickForm(ListPtr r, unsigned serial, bool marked, ListPtr old,
                long grade, bool allowDups, ListPtr otherFlavor);
    bool addCell(ListPtr r, CellFlavor f, unsigned serial, bool marked);
    bool addChord(ListPtr r);
    bool addScale(ListPtr r);

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
    std::array<ListPtr, 2> m_cells; // indexed by cell flavor
    std::array<ListPtr, 3> m_licks; // indexed by lick flavor
    ListPtr m_scales; 
    ListPtr m_chords; 
    ListPtr m_rhythms; 
    ListPtr m_ruleArray; // arrives via SetRules
    std::vector<ListPtr> m_markArray;

    /**
     * the table listing, for each chord, the chords it extends
     * (either directly, or indirectly)
     */
    ListPtr m_extensionsTable;
    ListPtr m_invertedExtensionsTable;

    unsigned m_serialNo; // used to number licks and cell advice items
    unsigned m_interChordTolerance; // half-beats, used in matching
    unsigned m_numChords;
    unsigned m_metre[2];
};