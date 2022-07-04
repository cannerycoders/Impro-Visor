#include "Advisor.h"
#include <cassert>

Advisor::Advisor()
{
    k_adviceTok = Polylist::symbol::getSymbol("advice");
    k_approachTok = Polylist::symbol::getSymbol("approach");
    k_brickTok = Polylist::symbol::getSymbol("brick");
    k_rhythmTok = Polylist::symbol::getSymbol("rhythm");
    k_cellTok = Polylist::symbol::getSymbol("cell");
    k_chordTok = Polylist::symbol::getSymbol("chord");
    k_chordsTok = Polylist::symbol::getSymbol("chords");
    k_colorTok = Polylist::symbol::getSymbol("color");
    k_drumPatternTok = Polylist::symbol::getSymbol("drum-pattern");
    k_extensionsTok = Polylist::symbol::getSymbol("extensions");
    k_familyTok = Polylist::symbol::getSymbol("family");
    k_gradeTok = Polylist::symbol::getSymbol("grade");
    k_idiomTok = Polylist::symbol::getSymbol("idiom");
    k_keyTok = Polylist::symbol::getSymbol("key");
    k_lickTok = Polylist::symbol::getSymbol("lick");
    k_markedTok = Polylist::symbol::getSymbol("marked");
    k_nameTok = Polylist::symbol::getSymbol("name");
    k_notesTok = Polylist::symbol::getSymbol("notes");
    k_priorityTok = Polylist::symbol::getSymbol("priority");
    k_quoteTok = Polylist::symbol::getSymbol("quote");
    k_sameTok = Polylist::symbol::getSymbol("same");
    k_scaleTok = Polylist::symbol::getSymbol("scale");
    k_scalesTok = Polylist::symbol::getSymbol("scales");
    k_sequenceTok = Polylist::symbol::getSymbol("sequence");
    k_serialTok = Polylist::symbol::getSymbol("serial");
    k_spellTok = Polylist::symbol::getSymbol("spell");
    k_styleTok = Polylist::symbol::getSymbol("style");
    k_substituteTok = Polylist::symbol::getSymbol("substitute");
    k_voicingsTok = Polylist::symbol::getSymbol("voicings");
    k_barTok = Polylist::symbol::getSymbol("|");
    k_slashTok = Polylist::symbol::getSymbol("/");
}

void
Advisor::SetRules(ListPtr rules)
{
    this->addRules(rules); 
}

void
Advisor::addRules(ListPtr rules)
{
    m_ruleArray = rules;
    m_markArray.reserve(rules->size());
    OListIt a = rules->getBegin();
    OListIt b = rules->getEnd();
    int i = 0;
    while(a != b)
    {
        ListPtr p = std::dynamic_pointer_cast<List>(*a);
        if(!p)
        {
            std::cerr << i << " unexpected type " << (*a)->getType() << "\n";
        }
        else
            this->addOneRule(p, i++, false/*marked*/, true/*allowDups*/);
        a++;
    }
}

/**
 * Add a single "rule" (chord, scale, rhythm, cell, idiom, lick, or quote)
 * to the database. Bricks and licks are the most populous.
 */
bool
Advisor::addOneRule(ListPtr r, unsigned serial, bool marked, bool allowDups)
{
    bool success=true;
    SymbolPtr s = r->firstSymbol();
    char const *sym = s->getValue();
    // std::cerr << "rule:" << sym << "\n";
    if(sym == k_brickTok)
    {
        return this->addLick(r, LickFlavor::k_Brick,
            serial, marked,
            marked || allowDups, LickFlavor::k_Plain);
    }
    else
    if(sym == k_lickTok)
    {
        return this->addLick(r, LickFlavor::k_Plain, serial, marked,
            marked || allowDups, LickFlavor::k_Quote);
    }
    else
    if(sym == k_quoteTok)
    {
        return this->addLick(r, LickFlavor::k_Quote, serial, marked,
            marked || allowDups, LickFlavor::k_Plain);
    }
    else
    if(sym == k_chordTok)
    {
        return this->addChord(r);
    }
    else
    if(sym == k_scaleTok)
    {
        return this->addScale(r);
    }
    else
    if(sym == k_styleTok)
    {
    }
    else
    if(sym == k_rhythmTok)
    {
        return this->addCell(r, CellFlavor::k_Rhythm, serial, marked);
    }
    else
    if(sym == k_cellTok)
    {
        return this->addCell(r, CellFlavor::k_Plain, serial, marked);
    }
    else
    if(sym == k_idiomTok)
    {
        return this->addCell(r, CellFlavor::k_Idiom, serial, marked);
    }
    else
    if(sym == k_markedTok)
    {
        // add one rule (rest)
    }
    else
    {
        std::cerr << serial << " " 
          << " WARNING: unidentified item attempted as a rule: " 
          <<  sym << "\n";
        success = false;
    }
    return success;
}

bool 
Advisor::addLick(ListPtr r, LickFlavor f, unsigned serial, bool marked,
                bool allowDups, LickFlavor otherFlavor)
{
    bool sucess = true;
    long grade = 0;
    auto list = r->findSublist(k_gradeTok);
    if(list)
    {
        auto x = list->getNth(1);
        LongPtr p = std::dynamic_pointer_cast<Long>(x);
        if(p)
            grade = p->getValue();
        else
            std::cerr << "addLick::getGrade fail\n";
    }

    ListPtr old = m_licks[(int) f];
    m_licks[(int)f] = this->addLickForm(r, serial, marked, old, 
                        grade, allowDups, m_licks[(int)otherFlavor]);
    return sucess;
}

Advisor::ListPtr 
Advisor::addLickForm(ListPtr r, unsigned serial, bool marked, ListPtr old,
                long grade, bool allowDups, ListPtr otherFlavor)
{
    ListPtr p;
    return p;
}

bool 
Advisor::addCell(ListPtr r, CellFlavor f, unsigned serial, bool marked)
{
    return false;
}

bool 
Advisor::addChord(ListPtr r)
{
    return false;
}

bool 
Advisor::addScale(ListPtr r)
{
    return false;
}