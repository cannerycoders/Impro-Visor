#include "Advisor.h"
#include "data/ChordForm.h"
#include "util/Log.h"

#include <cassert>

Advisor::Advisor()
{
    k_adviceTok = PlistSymbol::getSymbol("advice");
    k_approachTok = PlistSymbol::getSymbol("approach");
    k_brickTok = PlistSymbol::getSymbol("brick");
    k_rhythmTok = PlistSymbol::getSymbol("rhythm");
    k_cellTok = PlistSymbol::getSymbol("cell");
    k_chordTok = PlistSymbol::getSymbol("chord");
    k_chordsTok = PlistSymbol::getSymbol("chords");
    k_colorTok = PlistSymbol::getSymbol("color");
    k_drumPatternTok = PlistSymbol::getSymbol("drum-pattern");
    k_extensionsTok = PlistSymbol::getSymbol("extensions");
    k_familyTok = PlistSymbol::getSymbol("family");
    k_gradeTok = PlistSymbol::getSymbol("grade");
    k_idiomTok = PlistSymbol::getSymbol("idiom");
    k_keyTok = PlistSymbol::getSymbol("key");
    k_lickTok = PlistSymbol::getSymbol("lick");
    k_markedTok = PlistSymbol::getSymbol("marked");
    k_nameTok = PlistSymbol::getSymbol("name");
    k_notesTok = PlistSymbol::getSymbol("notes");
    k_priorityTok = PlistSymbol::getSymbol("priority");
    k_quoteTok = PlistSymbol::getSymbol("quote");
    k_sameTok = PlistSymbol::getSymbol("same");
    k_scaleTok = PlistSymbol::getSymbol("scale");
    k_scalesTok = PlistSymbol::getSymbol("scales");
    k_sequenceTok = PlistSymbol::getSymbol("sequence");
    k_serialTok = PlistSymbol::getSymbol("serial");
    k_spellTok = PlistSymbol::getSymbol("spell");
    k_styleTok = PlistSymbol::getSymbol("style");
    k_substituteTok = PlistSymbol::getSymbol("substitute");
    k_voicingsTok = PlistSymbol::getSymbol("voicings");
    k_barTok = PlistSymbol::getSymbol("|");
    k_slashTok = PlistSymbol::getSymbol("/");
}

void
Advisor::SetRules(Polylist::Ptr rules)
{
    m_ruleArray = rules;
    m_markArray.reserve(m_ruleArray->size());
    Polylist::ObjListIt a = rules->getBegin();
    Polylist::ObjListIt b = rules->getEnd();
    int i = 0;
    while(a != b)
    {
        this->addOneRule(*a, i++, false/*marked*/, true/*allowDups*/);
        a++;
    }
}

/**
 * Add a single "rule" (chord, scale, rhythm, cell, idiom, lick, or quote)
 * to the database. Bricks and licks are the most populous.
 */
bool
Advisor::addOneRule(Polylist::ObjPtr o, unsigned serial, bool marked, bool allowDups)
{
    Polylist *r = (Polylist *) o->asType(PlistObj::k_list);
    if(!r)
    {
        std::cerr << "Advisor: invalid rule " << o->getType() << "\n";
        return false;
    }
    bool success = true;
    auto  s = r->firstSymbol();
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
Advisor::addLick(Polylist *r, LickFlavor f, unsigned serial, bool marked,
                bool allowDups, LickFlavor otherFlavor)
{
    bool sucess = true;
    long grade = 0;
    auto list = r->findSublist(k_gradeTok);
    if(list)
    {
        auto x = list->getNth(1);
        PlistInt *p = static_cast<PlistInt *>(x->asType(Polylist::k_integer));
        if(p)
            grade = p->getValue();
        else
            std::cerr << "addLick::getGrade fail\n";
    }

    Polylist::Ptr old = m_licks[(int) f];
    m_licks[(int)f] = this->addLickForm(r, serial, marked, old, 
                        grade, allowDups, m_licks[(int)otherFlavor]);
    return sucess;
}

Polylist::Ptr
Advisor::addLickForm(Polylist *r, unsigned serial, bool marked, 
    Polylist::Ptr old, long grade, bool allowDups, 
    Polylist::Ptr otherFlavor)
{
    // assert(!"implementme");
    Polylist::Ptr p;
    return p;
}

bool 
Advisor::addCell(Polylist *r, CellFlavor f, unsigned serial, bool marked)
{
    return false;
}

/**
 * Create chord from raw polylist definition and add it to
 * the set of chords, indexed by name.  (Most of the creation
 * proper is done by makeChordForm.)
 */
bool 
Advisor::addChord(Polylist *chordDef)
{
    // Trace.log(2, "adding chord definition " + chordDef);
    auto nameEl = chordDef->findSymbol(k_nameTok);
    if(!nameEl)
    {
        Log::Log(Log::WARNING,
                "Chord definition %s needs a name (ignoring)",
                 chordDef->asString().c_str());
        return false;
    }

    ChordForm::Ptr form = ChordForm::makeChordForm(*chordDef);
    if(!form)
        return false;
    m_chords[nameEl->getValue()] = form;
    return true;
}

bool 
Advisor::addScale(Polylist *r)
{
    return false;
}