#include "ChordForm.h"
#include "polya/Polylist.h"
#include "Key.h"

char const * ChordForm::APPROACH = PListSymbol::getSymbol("approach");
char const * ChordForm::AVOID = PListSymbol::getSymbol("avoid");
char const * ChordForm::COLOR = PListSymbol::getSymbol("color");;
char const * ChordForm::EXTENSION = PListSymbol::getSymbol("extension");;
char const * ChordForm::EXTENSIONS = PListSymbol::getSymbol("extensions");;
char const * ChordForm::FAMILY = PListSymbol::getSymbol("family");;
char const * ChordForm::KEY = PListSymbol::getSymbol("key");;
char const * ChordForm::NAME = PListSymbol::getSymbol("name");;
char const * ChordForm::NOTES = PListSymbol::getSymbol("notes");;
char const * ChordForm::PRIORITY = PListSymbol::getSymbol("priority");;
char const * ChordForm::PRONOUNCE = PListSymbol::getSymbol("pronounce");;
char const * ChordForm::SAME = PListSymbol::getSymbol("same");;
char const * ChordForm::SCALES = PListSymbol::getSymbol("scales");;
char const * ChordForm::SPELL = PListSymbol::getSymbol("spell");;
char const * ChordForm::SUBS = PListSymbol::getSymbol("substitute");;
char const * ChordForm::TYPE = PListSymbol::getSymbol("type");;
char const * ChordForm::DEFAULT_FAMILY = PListSymbol::getSymbol("other");;
char const * ChordForm::VOICINGS = PListSymbol::getSymbol("voicings");;
char const * ChordForm::VOICING_WILD_CARD = PListSymbol::getSymbol("any");;

/* static */ChordForm::Ptr
ChordForm::makeChordForm(Polylist &plist)
{
    ChordForm::Ptr p;
    return p;
}

/* ------------------------------------------------------------------------ */
ChordForm::ChordForm()
{
    m_key = &Key::Ckey;
}