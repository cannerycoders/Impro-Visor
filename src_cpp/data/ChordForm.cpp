#include "ChordForm.h"
#include "polya/Polylist.h"
#include "Key.h"

char const * ChordForm::APPROACH = PlistSymbol::getSymbol("approach");
char const * ChordForm::AVOID = PlistSymbol::getSymbol("avoid");
char const * ChordForm::COLOR = PlistSymbol::getSymbol("color");;
char const * ChordForm::EXTENSION = PlistSymbol::getSymbol("extension");;
char const * ChordForm::EXTENSIONS = PlistSymbol::getSymbol("extensions");;
char const * ChordForm::FAMILY = PlistSymbol::getSymbol("family");;
char const * ChordForm::KEY = PlistSymbol::getSymbol("key");;
char const * ChordForm::NAME = PlistSymbol::getSymbol("name");;
char const * ChordForm::NOTES = PlistSymbol::getSymbol("notes");;
char const * ChordForm::PRIORITY = PlistSymbol::getSymbol("priority");;
char const * ChordForm::PRONOUNCE = PlistSymbol::getSymbol("pronounce");;
char const * ChordForm::SAME = PlistSymbol::getSymbol("same");;
char const * ChordForm::SCALES = PlistSymbol::getSymbol("scales");;
char const * ChordForm::SPELL = PlistSymbol::getSymbol("spell");;
char const * ChordForm::SUBS = PlistSymbol::getSymbol("substitute");;
char const * ChordForm::TYPE = PlistSymbol::getSymbol("type");;
char const * ChordForm::DEFAULT_FAMILY = PlistSymbol::getSymbol("other");;
char const * ChordForm::VOICINGS = PlistSymbol::getSymbol("voicings");;
char const * ChordForm::VOICING_WILD_CARD = PlistSymbol::getSymbol("any");;

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