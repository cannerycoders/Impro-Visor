#pragma once

#include "Constants.h"
#include "polya/Polylist.h"
#include <ostream>

class Preferences
{
public:
    static Preferences &get(); // singleton
    Preferences();

    void write(std::ostream &);
    std::string getPreference(char const *tok); // requires tokenized value
    std::string getPreference(std::string &nm);
    bool getAlwaysUseState();
    Constants::StaveType getStaveTypeFromPreferences();

public:
    static char const * DEFAULT_LOAD_STAVE;
    static char const * DEFAULT_MIXER_ALL;
    static char const * DEFAULT_MIXER_ENTRY;
    static char const * DEFAULT_MIXER_BASS;
    static char const * DEFAULT_MIXER_CHORDS;
    static char const * DEFAULT_MIXER_DRUMS;
    static char const * DEFAULT_MIXER_MELODY;
    static char const * DEFAULT_STYLE;
    static char const * DEFAULT_TEMPO;
    static char const * DEFAULT_VOCAB_FILE;
    static char const * DEFAULT_GRAMMAR_FILE;
    static char const * DEFAULT_TRANSFORM_FILE;
    static char const * DEFAULT_FRACTAL_FILE;
    static char const * DEFAULT_COUNTS_FILE;
    static char const * DEFAULT_STYLE_DIRECTORY;
    static char const * DEFAULT_CHORD_FONT_SIZE;

protected:
    Polylist m_prefs;
};