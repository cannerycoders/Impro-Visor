#include "Advisor.h"
#include <cassert>

Advisor::Advisor()
{
    k_adviceTok = Polylist::symbol::getToken("advice");
    k_approachTok = Polylist::symbol::getToken("approach");
    k_brickTok = Polylist::symbol::getToken("brick");
    k_rhythmTok = Polylist::symbol::getToken("rhythm");
    k_cellTok = Polylist::symbol::getToken("cell");
    k_chordTok = Polylist::symbol::getToken("chord");
    k_chordsTok = Polylist::symbol::getToken("chords");
    k_colorTok = Polylist::symbol::getToken("color");
    k_drumPatternTok = Polylist::symbol::getToken("drum-pattern");
    k_extensionsTok = Polylist::symbol::getToken("extensions");
    k_familyTok = Polylist::symbol::getToken("family");
    k_gradeTok = Polylist::symbol::getToken("grade");
    k_idiomTok = Polylist::symbol::getToken("idiom");
    k_keyTok = Polylist::symbol::getToken("key");
    k_lickTok = Polylist::symbol::getToken("lick");
    k_markedTok = Polylist::symbol::getToken("marked");
    k_nameTok = Polylist::symbol::getToken("name");
    k_notesTok = Polylist::symbol::getToken("notes");
    k_priorityTok = Polylist::symbol::getToken("priority");
    k_quoteTok = Polylist::symbol::getToken("quote");
    k_sameTok = Polylist::symbol::getToken("same");
    k_scaleTok = Polylist::symbol::getToken("scale");
    k_scalesTok = Polylist::symbol::getToken("scales");
    k_sequenceTok = Polylist::symbol::getToken("sequence");
    k_serialTok = Polylist::symbol::getToken("serial");
    k_spellTok = Polylist::symbol::getToken("spell");
    k_styleTok = Polylist::symbol::getToken("style");
    k_substituteTok = Polylist::symbol::getToken("substitute");
    k_voicingsTok = Polylist::symbol::getToken("voicings");
    k_barTok = Polylist::symbol::getToken("|");
    k_slashTok = Polylist::symbol::getToken("/");
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