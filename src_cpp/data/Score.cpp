#include "Score.h"
#include "util/Preferences.h"

/* static ---------------------------------------------  */
std::string Score::DEFAULT_BARS_PER_LINE = "4";
std::string Score::DEFAULT_TITLE = "";
std::string Score::DEFAULT_COMPOSER = "";
const float Score::DEFAULT_TEMPO = (float) Preferences::get().getDoublePref(Preferences::DEFAULT_TEMPO);
const int Score::DEFAULT_VOLUME = Constants::MAX_VOLUME;
const int Score::DEFAULT_KEYSIG = 0;

/* -------------------------------------------------  */

Score::Score(std::string const & title, float tempo, int volume)
{
    m_title = title;
    m_tempo = tempo;
    m_masterVolume = volume;
    m_length = 0;

    m_composer = DEFAULT_COMPOSER;
    m_metre = Constants::DEFAULT_METRE;

    m_breakpoint = Constants::FS3; 
    m_transposition = Transposition(); // none
    m_chordFontSize = 16;
    m_constantBass = false;
    m_roadmapLayout = 8;

    m_chordVolume = 60;
    m_bassVolume = 60;
    m_drumVolume = 60;
    m_melodyVolume = Constants::MAX_VOLUME;
    m_chordMuted = false;
    m_bassMuted = false;
    m_drumMuted = false;
    m_melodyMuted = false;
    m_masterVolumeMuted = false;
    
    m_bassInstrument = 34; // DEFAULT_BASS_INSTRUMENT;
}

void
Score::setScoreItemsFromPrefs()
{
    Preferences &prefs = Preferences::get();
    setChordFontSize(prefs.getPref(Preferences::DEFAULT_CHORD_FONT_SIZE));

    // user instruments (from General MIDI) have 1 index origin. 
    setMelodyInstrument(prefs.getPref(Preferences::DEFAULT_MELODY_INSTRUMENT) - 1); // ??
    setBassInstrument(prefs.getPref(Preferences::DEFAULT_BASS_INSTRUMENT) - 1); // ??
    setChordInstrument(prefs.getPref(Preferences::DEFAULT_CHORD_INSTRUMENT) - 1);
    
    int bassTrans = prefs.getPref(Preferences::DEFAULT_BASS_TRANSPOSITION);
    int chordTrans = prefs.getPref(Preferences::DEFAULT_CHORD_TRANSPOSITION);
    int melodyTrans = prefs.getPref(Preferences::DEFAULT_MELODY_TRANSPOSITION);
    m_transposition = Transposition(bassTrans, chordTrans, melodyTrans);        
}

int 
Score::getActiveBarsInChorus()
{
    int activeBars = m_chordProg->getActiveBars();
    ListIterator<MelodyPart> i = partList.listIterator();
    while(i.hasNext())
    {
        int barsInChorus = i.next().getActiveBars();
        if( barsInChorus > activeBars )
        {
            activeBars = barsInChorus;
        }
    }
    //System.out.println("active bars = " + activeBars);
    return activeBars; 
}