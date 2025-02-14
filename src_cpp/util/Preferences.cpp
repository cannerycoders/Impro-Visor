#include "Preferences.h"

static char const *s_defaultPrefs = 
    "(rhythm-cluster-filename default.cluster)"
    "(my-rhythms-file My.rhythms)"
    "(default-auxeinstrument 57)"
    "(audio-in-latency 1.0)"
    "(advice-cache-size 10)"
    "(cache-enabled true)"
    "(default-load-stave 1)"
    "(default-melody-instrument 12)"
    "(default-chord-instrument 1)"
    "(default-bass-instrument 34)"
    "(default-melody-transposition 0)"
    "(default-chord-transposition 0)"
    "(default-bass-transposition 0)"
    "(default-mixer-all 80)"
    "(default-mixer-entry 50)"
    "(default-mixer-bass 100)"
    "(default-mixer-chords 100)"
    "(default-mixer-drums 100)"
    "(default-mixer-melody 127)"
    "(default-style swing)"
    "(default-tempo 160.0)"
    "(default-vocab-file My.voc)"
    "(default-transform-file My.transform)"
    "(default-counts-file ALL_MUSICIANS.counts)"
    "(default-fractal-file My.fractal)"
    "(default-style-directory styles)"
    "(visible-advice-components 4095)"
    "(chord-dist-above-root 10)"
    "(default-chord-font-size 16)"
    "(max-notes-in-voicing 5)"
    "(note-coloring 1342)"
    "(show-tracking-line y)"
    "(tracker-delay 0)"
    "(contour-drawing-tones 1xx)"
    "(always-use-buttons nnnn)" // are y or n for [CHORD, BASS, DRUMS, STAVE]
    "(create-roadmap n)"
    "(midi-in RealTimeSequencer)"
    "(midi-out Gervill)"
    "(midi-echo y)"
    "(midi-in-latency 0.0)"
    "(midi-record-snap 2)"
    "(midi-send-bank-select n)"
    "(melody-channel 1)"
    "(chord-channel 4)"
    "(bass-channel 7)"
    "(drum-channel 10)"
    "(recent-style-file swing.sty)"
    "(improv-menu-setting (Use Improvise Button))"
    "(language en)"
    "(language_list (en , fr))"
    "(lstm-gen-params combination.ctome)"
    // Be careful: Outer parens needed below
    "(transposing-instruments ("
        "(No-Transposition 0 0 Treble 0)"
        "(Bb-Trumpet -2 -2 Treble 2)"
        "(Bb-TenorSax -14 -2 Treble 2)" 
        "(Bb-SopranoSax -2 -2 Treble 2)"
        "(Eb-AltoSax -9 3 Treble 9)"
        "(Eb-BaritoneSax -21 3 Treble 9)"
        "(F-Horn -5 -5 Treble -5)"
        "(Trombone -12 0 Bass -12)"
        "(SopranoRecorder 12 0 Treble 0)"
        "(BassRecorder -12 0 Bass -12)"
        "))"
        // Be careful: Outer parens needed below
    "(roadmap-colors ("
        "(250 220 100)" // C
        "(247 126 255)" // Db
        "(150 255   0)" // D
        "(255 182 180)" // Eb
        "(131 235 255)" // E
        "(255 221 118)" // F
        "(169 184 250)" // Gb
        "(255 255   0)" // G
        "(255 189 255)" // Ab
        "(150 255 202)" // A
        "(255 217 150)" // Bb
        "(157 209 255)" // B
    "))";

typedef Preferences P;

// defer init 'til first get()
// windows "crash" due to apparent race between static Polylist::symbol init
char const * P::DEFAULT_LOAD_STAVE = nullptr; 
char const * P::DEFAULT_MIXER_ALL  = nullptr;
char const * P::DEFAULT_MIXER_ENTRY  = nullptr;
char const * P::DEFAULT_MIXER_BASS  = nullptr;
char const * P::DEFAULT_MIXER_CHORDS  = nullptr;
char const * P::DEFAULT_MIXER_DRUMS  = nullptr;
char const * P::DEFAULT_MIXER_MELODY  = nullptr;
char const * P::DEFAULT_STYLE  = nullptr;
char const * P::DEFAULT_TEMPO  = nullptr;
char const * P::DEFAULT_VOCAB_FILE  = nullptr;
char const * P::DEFAULT_GRAMMAR_FILE  = nullptr;
char const * P::DEFAULT_TRANSFORM_FILE  = nullptr;
char const * P::DEFAULT_FRACTAL_FILE  = nullptr;
char const * P::DEFAULT_COUNTS_FILE  = nullptr;
char const * P::DEFAULT_STYLE_DIRECTORY  = nullptr;
char const * P::DEFAULT_CHORD_FONT_SIZE  = nullptr;
char const * P::ALWAYS_USE_BUTTONS = nullptr;

char const * P::DEFAULT_MELODY_INSTRUMENT;
char const * P::DEFAULT_AUX_INSTRUMENT;
char const * P::DEFAULT_CHORD_INSTRUMENT;
char const * P::DEFAULT_BASS_INSTRUMENT;

char const * P::DEFAULT_MELODY_TRANSPOSITION;
char const * P::DEFAULT_AUX_TRANSPOSITION;
char const * P::DEFAULT_CHORD_TRANSPOSITION;
char const * P::DEFAULT_BASS_TRANSPOSITION;

/*static*/ Preferences &
Preferences::get()
{
    static int init=0;
    static Preferences p;
    if(init == 0)
    {
        init = 1;
        P::DEFAULT_LOAD_STAVE = PlistSymbol::getSymbol("default-load-stave");
        P::DEFAULT_MIXER_ALL = PlistSymbol::getSymbol("default-mixer-all");
        P::DEFAULT_MIXER_ENTRY = PlistSymbol::getSymbol("default-mixer-entry");
        P::DEFAULT_MIXER_BASS = PlistSymbol::getSymbol("default-mixer-bass");
        P::DEFAULT_MIXER_CHORDS = PlistSymbol::getSymbol("default-mixer-chords");
        P::DEFAULT_MIXER_DRUMS = PlistSymbol::getSymbol("default-mixer-drums");
        P::DEFAULT_MIXER_MELODY = PlistSymbol::getSymbol("default-mixer-melody");
        P::DEFAULT_STYLE = PlistSymbol::getSymbol("default-style");
        P::DEFAULT_TEMPO = PlistSymbol::getSymbol("default-tempo");
        P::DEFAULT_VOCAB_FILE = PlistSymbol::getSymbol("default-vocab-file");
        P::DEFAULT_GRAMMAR_FILE = PlistSymbol::getSymbol("default-grammar-file");
        P::DEFAULT_TRANSFORM_FILE = PlistSymbol::getSymbol("default-transform-file");
        P::DEFAULT_FRACTAL_FILE = PlistSymbol::getSymbol("default-fractal-file");
        P::DEFAULT_COUNTS_FILE = PlistSymbol::getSymbol("default-counts-file");
        P::DEFAULT_STYLE_DIRECTORY = PlistSymbol::getSymbol("default-style-directory");
        P::DEFAULT_CHORD_FONT_SIZE = PlistSymbol::getSymbol("default-chord-font-size");
        P::ALWAYS_USE_BUTTONS = PlistSymbol::getSymbol("always-use-buttons");

        P::DEFAULT_MELODY_INSTRUMENT = PlistSymbol::getSymbol("default-melody-instrument");
        P::DEFAULT_AUX_INSTRUMENT = PlistSymbol::getSymbol("default-aux-instrument");
        P::DEFAULT_CHORD_INSTRUMENT = PlistSymbol::getSymbol("default-chord-instrument");
        P::DEFAULT_BASS_INSTRUMENT = PlistSymbol::getSymbol("default-bass-instrument");

        P::DEFAULT_MELODY_TRANSPOSITION = PlistSymbol::getSymbol("default-melody-transposition");
        P::DEFAULT_AUX_TRANSPOSITION = PlistSymbol::getSymbol("default-aux-transposition");
        P::DEFAULT_CHORD_TRANSPOSITION = PlistSymbol::getSymbol("default-chord-transposition");
        P::DEFAULT_BASS_TRANSPOSITION = PlistSymbol::getSymbol("default-bass-transposition");
    }
    return p;
}

/* ------------------------------------------------------- */
Preferences::Preferences()
{
    m_prefs.Parse(s_defaultPrefs); //My.prefs string into polylist
}

void
Preferences::write(std::ostream &o)
{
    m_prefs.write(o);
}

std::string 
Preferences::getPreference(std::string &p)
{
    return getPreference(PlistSymbol::getSymbol(p));
}

long
Preferences::getLongPref(char const *nm)
{
    return std::stol(getPreference(nm));
}

double
Preferences::getDoublePref(char const *nm)
{
    return std::stod(getPreference(nm));
}

std::string 
Preferences::getPreference(char const *nm)
{
    std::string value;

    auto l = m_prefs.findSublist(nm);
    if(l)
        value = l->getNth(1)->asString();
    else
    {
        std::cerr << "Preference " <<  nm <<
            " did not exist in user preferences.\n";
    }
    return value;
}


// a string: 'nnnn' [CHORD, BASS, DRUMS, STAVE]
bool 
Preferences::getAlwaysUse(int index) 
{
    std::string alwaysUseButtons = getPreference(ALWAYS_USE_BUTTONS);
    if(index >= alwaysUseButtons.length())
        return false;
    return alwaysUseButtons[index] == 'y';
}

bool 
Preferences::getAlwaysUseStave()
{
    return getAlwaysUse(3);
}

Constants::StaveType 
Preferences::getStaveType()
{
    long stave = getLongPref(DEFAULT_LOAD_STAVE);
    return (Constants::StaveType) stave; // 1 is default (Treble)
}