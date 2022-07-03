#pragma once

#include <string>
#include <vector>

/**
 * A namespace to hold all Impro-Visor shared constants.
 */
namespace Constants
{
    extern const std::string LEADSHEET_FILE_EXTENSION;
    extern const std::string SAVE_IMPROVISATION_STEM;
    extern const std::string SAVE_IMPROVISATION_EXTENSION;

    extern const int MAX_VOLUME;
    extern const int NUM_CHANNELS;
    extern const int MAX_BEATS_PER_BAR;
    extern const int DEFAULT_BEATS_PER_BAR;
    extern const int MAX_BEAT_DENOMINATOR;
    extern const int DEFAULT_BEAT_DENOMINATOR;
    
    /**
     *  120 slots in a single beat
     */
    extern const int BEAT;
    extern const int THIRD_BEAT;
    extern const int HALF_BEAT;
    extern const int SIXTH_BEAT;
    
    /**
     * CUSTOM is used to indicate custom voicings
     */
    
    extern const std::string CUSTOM;
    
    /** Note: All of the following must have no fractional remainder,
     *  Hence no SIXTYFOURTH for example. SIXTYFOURTH_TRIPLET is possible
     *  in principle, but we don't have an image for it yet.
     */
    
    extern const int WHOLE; // slots in a whole note
    extern const int HALF;
    extern const int QUARTER;
    extern const int EIGHTH;
    extern const int SIXTEENTH;
    extern const int THIRTYSECOND;
   
    extern const int HALF_TRIPLET;
    extern const int QUARTER_TRIPLET;
    extern const int EIGHTH_TRIPLET;
    extern const int SIXTEENTH_TRIPLET;
    extern const int THIRTYSECOND_TRIPLET;

    extern const int QUARTER_QUINTUPLET;
    extern const int EIGHTH_QUINTUPLET;
    extern const int SIXTEENTH_QUINTUPLET;
    extern const int THIRTYSECOND_QUINTUPLET;

    extern const int DOTTED_HALF;
    extern const int DOTTED_QUARTER;
    extern const int DOTTED_EIGHTH;
    extern const int DOTTED_SIXTEENTH;

    extern const int FOUREIGHTIETH;
    extern const int TWOFORTIETH;
    extern const int ONETWENTIETH;
    extern const int SIXTIETH;

    extern const int SIXTYFOURTH_TRIPLET;
   
    /**
     * Note: These must be in ascending order for binary search to work.
    * What about double dotted?
    */ 
    extern std::vector<int> const knownNoteValue;
  
    /**
     * Note: These  must be in ascending order for binary search to work.
    * Rests are not dotted.
    */ 
    extern std::vector<int> const knownRestValue;

    /**
     * These are the values that can be used in tuplets, either as rests or notes.
    * Note: These must be in ascending order for binary search to work.
    */ 
    extern std::vector<int> const knownTupletValue;
  
    extern std::vector<int> const knownResolutionValue;
  

    extern const int DEFAULT_DURATION;	// eighth note value

    extern const int DEFAULT_METRE[2];

    /**
     * Types of notes
     */

    extern const int CHORD_TONE;
    extern const int COLOR_TONE;
    extern const int APPROACH_TONE;
    extern const int FOREIGN_TONE;
    
    /**
     * Colors for the notes
     */

    extern const int BLACK;
    extern const int RED;
    extern const int GREEN;
    extern const int BLUE;
    
    /**
     * Enharmonics
     */    
    extern const int CSHARP;
    extern const int DSHARP;
    extern const int FSHARP;
    extern const int GSHARP;
    extern const int ASHARP;
    
    /**
     * Which portions of the leadsheet to export
     */
    extern const int ALL;
    extern const int CHORDS_ONLY;
    extern const int MELODY_ONLY;
    
    /**
     * the minimum allowed rhythm value (not fully implemented)
     */
    extern const int MIN_RHYTHM_VALUE;

    /**
     * the value indicating a variable is out of bounds or invalid
     */
    extern const int OUT_OF_BOUNDS;
    
    /**
     * the "pitch" of a rest
     */
    extern const int REST;

    /**
     * enum indicating which stave the current window should display
     */
    enum class StaveType {GRAND, TREBLE, BASS, AUTO, NONE};
    
    /**
     * a flag so a Note knows whether it should be sharp, flat, or natural
     */
    enum class Accidental {NATURAL, SHARP, FLAT, NOTHING};

    /**
     * Positions of black notes in the chromatic scale (starting at 0)
     */

    extern const std::vector<int> blackNotes;


    /** "minimum" key signature used by Impro-Visor is six flats (Gb) */

    extern const int MIN_KEY;


    /** "maximum" key signature used by Impro-Visor is seven sharps (C#) */

    extern const int MAX_KEY;
    
    extern const int MIN_TS;
    extern const int MAX_TS;


    /**
     * The number of semitones in an octave.
     */

    extern const int OCTAVE;


    /**
     * The one-and-only symbol for no chord
     */

    extern std::string const NOCHORD;


    /**
     * The bar (measure) symbol
     */

    extern std::string const BARString;


    /**
     * Comma is also usable as a measure symbol
     */

    extern std::string const COMMAString;


    /**
     * The slash symbol, used for slash-chords and chord repetition
     */

    extern std::string const SLASHString;


    /**
     * The common root for chord table lookup.
     */

    extern std::string const CROOT;

    /**
     * Lower case version of pitch class or root c.
     */

    extern std::string const LCROOT;

    /**
     * The default duration of a note when none is specified.
     */


    extern std::string const DEFAULT_DURATION_MSG;

    /**
     * The midi number for middle C
     */

    extern const int CMIDI;


    /**
     * The std::string representing a rest
     */

    extern const std::string REST_String;


    /**
     * The std::string representing an eighth-rest
     */

    extern const std::string EIGHTH_REST_String;


    /**
     * Input and output symbols used for chords and notes
     */

    extern const char SHARP;
    extern const char FLAT;
    extern const char BAR;
    extern const char COMMA;
    extern const char SLASH;
    extern const char BACKSLASH;
    extern const char DOT;
    extern const char PLUS;
    extern const char MINUS;
    extern const char RESTCHAR;

    /**
     * Modes of extracting stuff for transfer
     */

    enum ExtractMode {RHYTHM, CELL, IDIOM, LICK, QUOTE, BRICK, CHORDS, MELODY, BOTH};


    /**
     * maps absolute notes to MIDI numbers
     */
    extern const int
           G9,
           GF9,
           FS9,
           F9,
           FF9,
           ES9,
           E9,
           EF9,
           DS9,
           D9,
           DF9,
           CS9,
           C9,
           CF9,
           BS8,
           B8,
           BF8,
           AS8,
           A8,
           AF8,
           GS8,
           G8,
           GF8,
           FS8,
           F8,
           FF8,
           ES8,
           E8,
           EF8,
           DS8,
           D8,
           DF8,
           CS8,
           C8,
           CF8,
           BS7,
           B7,
           BF7,
           AS7,
           A7,
           AF7,
           GS7,
           G7,
           GF7,
           FS7,
           F7,
           FF7,
           ES7,
           E7,
           EF7,
           DS7,
           D7,
           DF7,
           CS7,
           C7,
           CF7,
           BS6,
           B6,
           BF6,
           AS6,
           A6,
           AF6,
           GS6,
           G6,
           GF6,
           FS6,
           F6,
           FF6,
           ES6,
           E6,
           EF6,
           DS6,
           D6,
           DF6,
           CS6,
           C6,
           CF6,
           BS5,
           B5,
           BF5,
           AS5,
           A5,
           AF5,
           GS5,
           G5,
           GF5,
           FS5,
           F5,
           FF5,
           ES5,
           E5,
           EF5,
           DS5,
           D5,
           DF5,
           CS5,
           C5,
           CF5,
           BS4,
           B4,
           BF4,
           AS4,
           A4,
           AF4,
           GS4,
           G4,
           GF4,
           FS4,
           F4,
           FF4,
           ES4,
           E4,
           EF4,
           DS4,
           D4,
           DF4,
           CS4,
           C4,
           CF4,
           BS3,
           B3,
           BF3,
           AS3,
           A3,
           AF3,
           GS3,
           G3,
           GF3,
           FS3,
           F3,
           FF3,
           ES3,
           E3,
           EF3,
           DS3,
           D3,
           DF3,
           CS3,
           C3,
           CF3,
           BS2,
           B2,
           BF2,
           AS2,
           A2,
           AF2,
           GS2,
           G2,
           GF2,
           FS2,
           F2,
           FF2,
           ES2,
           E2,
           EF2,
           DS2,
           D2,
           DF2,
           CS2,
           C2,
           CF2,
           BS1,
           B1,
           BF1,
           AS1,
           A1,
           AF1,
           GS1,
           G1,
           GF1,
           FS1,
           F1,
           FF1,
           ES1,
           E1,
           EF1,
           DS1,
           D1,
           DF1,
           CS1,
           C1,
           CF1,
           BS0,
           B0,
           BF0,
           AS0,
           A0,
           AF0,
           GS0,
           G0,
           GF0,
           FS0,
           F0,
           FF0,
           ES0,
           E0,
           EF0,
           DS0,
           D0,
           DF0,
           CS0,
           C0,
           CF0,
           BSN1,
           BN1,
           BFN1,
           ASN1,
           AN1,
           AFN1,
           GSN1,
           GN1,
           GFN1,
           FSN1,
           FN1,
           FFN1,
           ESN1,
           EN1,
           EFN1,
           DSN1,
           DN1,
           DFN1,
           CSN1,
           CN1,
           g9,
           gf9,
           fs9,
           f9,
           ff9,
           es9,
           e9,
           ef9,
           ds9,
           d9,
           df9,
           cs9,
           c9,
           cf9,
           bs8,
           b8,
           bf8,
           as8,
           a8,
           af8,
           gs8,
           g8,
           gf8,
           fs8,
           f8,
           ff8,
           es8,
           e8,
           ef8,
           ds8,
           d8,
           df8,
           cs8,
           c8,
           cf8,
           bs7,
           b7,
           bf7,
           as7,
           a7,
           af7,
           gs7,
           g7,
           gf7,
           fs7,
           f7,
           ff7,
           es7,
           e7,
           ef7,
           ds7,
           d7,
           df7,
           cs7,
           c7,
           cf7,
           bs6,
           b6,
           bf6,
           as6,
           a6,
           af6,
           gs6,
           g6,
           gf6,
           fs6,
           f6,
           ff6,
           es6,
           e6,
           ef6,
           ds6,
           d6,
           df6,
           cs6,
           c6,
           cf6,
           bs5,
           b5,
           bf5,
           as5,
           a5,
           af5,
           gs5,
           g5,
           gf5,
           fs5,
           f5,
           ff5,
           es5,
           e5,
           ef5,
           ds5,
           d5,
           df5,
           cs5,
           c5,
           cf5,
           bs4,
           b4,
           bf4,
           as4,
           a4,
           af4,
           gs4,
           g4,
           gf4,
           fs4,
           f4,
           ff4,
           es4,
           e4,
           ef4,
           ds4,
           d4,
           df4,
           cs4,
           c4,
           cf4,
           bs3,
           b3,
           bf3,
           as3,
           a3,
           af3,
           gs3,
           g3,
           gf3,
           fs3,
           f3,
           ff3,
           es3,
           e3,
           ef3,
           ds3,
           d3,
           df3,
           cs3,
           c3,
           cf3,
           bs2,
           b2,
           bf2,
           as2,
           a2,
           af2,
           gs2,
           g2,
           gf2,
           fs2,
           f2,
           ff2,
           es2,
           e2,
           ef2,
           ds2,
           d2,
           df2,
           cs2,
           c2,
           cf2,
           bs1,
           b1,
           bf1,
           as1,
           a1,
           af1,
           gs1,
           g1,
           gf1,
           fs1,
           f1,
           ff1,
           es1,
           e1,
           ef1,
           ds1,
           d1,
           df1,
           cs1,
           c1,
           cf1,
           bs0,
           b0,
           bf0,
           as0,
           a0,
           af0,
           gs0,
           g0,
           gf0,
           fs0,
           f0,
           ff0,
           es0,
           e0,
           ef0,
           ds0,
           d0,
           df0,
           cs0,
           c0,
           cf0,
           bsn1,
           bn1,
           bfn1,
           asn1,
           an1,
           afn1,
           gsn1,
           gn1,
           gfn1,
           fsn1,
           fn1,
           ffn1,
           esn1,
           en1,
           efn1,
           dsn1,
           dn1,
           dfn1,
           csn1,
           cn1;

    extern const int TOTALPITCHES;
    extern const int SEMITONES;
    
    extern const int MIN_PITCH;
    extern const int MAX_PITCH;
    extern const int MIN_INTERVAL_SIZE;
    extern const int MAX_INTERVAL_SIZE;
    extern const int MIN_NOTE_DURATION;
    extern const int MAX_NOTE_DURATION;

    extern const int MODF,
                            MODC,
                            MODG,
                            MODD,
                            MODA,
                            MODE,
                            MODB;

    extern const int CBMAJOR,
                            GBMAJOR,
                            DBMAJOR,
                            ABMAJOR,
                            EBMAJOR,
                            BBMAJOR,
                            FMAJOR,
                            CMAJOR,
                            GMAJOR,
                            DMAJOR,
                            AMAJOR,
                            EMAJOR,
                            BMAJOR,
                            FSMAJOR,
                            CSMAJOR;
    
    // Terminal values for the grammar
    extern const char T_NOTE,
			       T_CHORD,
                   T_SCALE,
			       T_COLOR,
			       T_APPROACH,
			       T_REST,
                   T_RANDOM,
                   T_OUTSIDE,
                   T_EXPECTANCY,
                   T_BASS,
                   T_GOAL;

    extern const std::string NONE;
    extern const std::string FIRST_SCALE;
    
    extern const int IGNORE, OVERWRITE, SAVE;
    
    extern const std::string STYLE_EXTENSION;
    
    extern const std::string VOICING_REDIRECT_PREFIX;
    extern const std::string GENERATED_VOICING_NAME;
    extern const std::string GENERATED_VOICING_TYPE;

    /**
     * Used to indicate score is to be played to end.
     */
    extern const int ENDSCORE;
};