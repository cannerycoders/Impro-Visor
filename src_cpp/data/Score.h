#pragma once

#include "Constants.h"
#include "ChordPart.h"
#include "MelodyPart.h"
#include "Transposition.h"
#include "polya/Polylist.h"

#include <string>
#include <array>

/**
 * The Score class is representative of a musical score, containing several
 * parallel Parts, including melodies and chord progressions.
 * A Score contains several Parts stored in a PartList.
 * It contains information about the total Score, such as volume, tempo,
 * and title.  Parts should be added with the addPart method.
 */
class Score
{
public:
    /**
     * Default bars per line layout. Note that, in general, the layout
     * specification is a list of bars per line, the last of which is used
     * if there are more lines than list entries.
     */
    static std::string DEFAULT_BARS_PER_LINE;
    static std::string DEFAULT_TITLE;
    static std::string DEFAULT_COMPOSER;
    static const float DEFAULT_TEMPO;
    static const int DEFAULT_VOLUME;
    static const int DEFAULT_KEYSIG;

public:
    Score() : Score(DEFAULT_TITLE, DEFAULT_TEMPO, DEFAULT_VOLUME) {}
    Score(float tempo) : Score(DEFAULT_TITLE, tempo, DEFAULT_VOLUME) {}
    Score(std::string const &title, float tempo) : Score(title, tempo, DEFAULT_VOLUME) {}
    Score(std::string const & title, float tempo, int volume);

    void setScoreItemsFromPrefs();
    void setConstantBass(bool x) { m_constantBase = x; }
    void setCountIn(ChordPart::Ptr &countInProg x) { m_countInProg = x;}
    void noCountIn() { m_countInProg.reset(); }

    Transposition const &getTransposition() { return m_transposition; }
    void setTransposition(Transposition &x) { m_transposition = x; }
    int getChordFontSize() { return m_chordFontSize; }
    void setChordFontSize(int x) { m_chordFontSize = x; }
    int getBarsPerChorus() { return m_chordProg->getBars(); }
    int getActiveBarsInChorus();

    void setBassInstrument(int x) { m_bassInstrument = x; }
    void setChordInstrument(int x) { m_bassInstrument = x; }


private:
    // Sequence m_cachedSequence ()
    //  cf: import javax.sound.midi.Sequence;

    /**
     * The title of the Score
     */
    std::string m_title;

    /**
     * The composer of the Score
     */
    std::string m_composer;

    /**
     * The name of the show, if any
     */
    std::string m_showTitle;

    /**
     * The year, if specified
     */
    std::string m_year;

    /**
     * Comments on this score
     */
    std::string m_comments;

    /**
     * The length of the Score
     */
    int m_length;
    
    /**
     * The metre of the score
     */
    std::array<int, 2> m_metre;
    
    /**
     * The key signature of the score
     */
    int m_keySig;
    
    /**
     * The breakpoint pitch indicating where the treble stave should start drawing and
     * the bass stave should end drawing, or vice versa.
     */
    int m_breakpoint; //  FS3;
    
    /**
     * The Parts in the Score
     */
    Part::List m_partList;

    /**
     * The chord Progression (required)
     */
    ChordPart m_chordProg;

    /**
     * The count-in Progression (optional)
     */
    ChortPart::Ptr m_countInProg; 

    /**
     * The tempo of the Score
     */
    float m_tempo;

    /**
     * The playback transposition of the Score
     */
    Transposition m_transposition;

    int m_chordFontSize;

    std::string m_voicingType;
    
    /**
     * A list of voicings for the chords, for stepping purposes
     */
    bool m_constantBass;

    /**
     * The layout, if any. This can be null if no layout specified.
     */
    Polylist m_layout;
    
    /**
     * Layout for generated roadmap. Currently it is only a single number,
     * bars per line.
     */
    int m_roadmapLayout; // 8

    int m_chordVolume;
    int m_bassVolume;
    int m_drumVolume;
    int m_melodyVolume;
    int m_masterVolume;
    bool m_chordMuted;
    bool m_bassMuted;
    bool m_drumMuted;
    bool m_melodyMuted;
    bool m_masterVolumeMuted;
    int m_bassInstrument;
};