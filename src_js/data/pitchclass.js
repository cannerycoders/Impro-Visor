import Constants from "../constants.js";

/**
 * The PitchClass class deals with immutable pitch classes, which are like pitches but
 * without a specific octave.
 */

const sPitches = ["a", "b", "c", "d", "e", "f", "g"]; // pitches are lower, chords are upper
const sPitchChrs = sPitches.map(v => v.charCodeAt(0));

export class PitchClass
{
    /*------------------------------------------------------------------------ */
    /* 20 PitchClasses some of which are enharmonic 
     */
    static pitchClass = 
    [
        //        name, index, semis, natural, #, #pref, chord
        new PitchClass("fb",  0,  4, false, false, false, "E"),
        new PitchClass("cb",  1, 11, false, false, false, "B"), // prefer b
        new PitchClass("gb",  2,  6, false, false, false, "Gb"),
        new PitchClass("db",  3,  1, false, false, false, "Db"),
        new PitchClass("ab",  4,  8, false, false, false, "Ab"),
        new PitchClass("eb",  5,  3, false, false, false, "Eb"),
        new PitchClass("bb",  6, 10, false, false, false, "Bb"),
        new PitchClass("f",   7,  5,  true, false, false, "F"),
        new PitchClass("c",   8,  0,  true, false, false, "C"),
        new PitchClass("g",   9,  7,  true, false, true,  "G"), 
        new PitchClass("d",  10,  2,  true, false, true,  "D"),
        new PitchClass("a",  11,  9,  true, false, true,  "A"),
        new PitchClass("e",  12,  4,  true, false, true,  "E"),
        new PitchClass("b",  13, 11,  true, false, true,  "B"),
        new PitchClass("f#", 14,  6, false, true,  true,  "F#"),
        new PitchClass("c#", 15,  1, false, true,  true,  "C#"),
        new PitchClass("g#", 16,  8, false, true,  false, "Ab"), // prefer ab
        new PitchClass("d#", 17,  3, false, true,  false, "Eb"), // prefer eb
        new PitchClass("a#", 18, 10, false, true,  false, "Bb"), // prefer bb
        new PitchClass("e#", 19,  5, false, true,  false, "F"),  // prefer f
        new PitchClass("b#", 20,  0, false, true,  false, "C")   // prefer c
    ];

    static upperCaseNote(s)
    {
        return s.toUpper(); // works for +,-,N, ...
    }

    static isValidPitch(s)
    {
        return this.getPitchClass(s) != null;
    }

    static isValidPitchStart(s)
    {
        if(typeof(s) === 'number')
            return sPitchChrs.indexOf(s) != -1;
        else
            return sPitches.indexOf(s);
    }

    static getPitchClass(s)
    {
        if(s.length == 0) return null;
        let lcName = s.toLowerCase();
        return this.pitchClass[this[lcName]];
    }

    /*------------------------------------------------------------------------ */
    constructor(nm, index, semisAboveC, natural,
                sharp, sharpPref, chordBase)
    {
        this.name = nm;
        this.index = index;
        this.semitonesAboveC = semisAboveC;
        this.natural = natural;
        this.sharp = sharp;
        this.sharpPref = sharpPref;
        this.chordBase = chordBase;

        /* install this into class, accessed like PitchClass.f, etc */
        this.constructor[this.name] = index;
        if(nm.endsWith("#"))
        {
            // alias PitchClass.f# to PitchClass.fs
            this.constructor[this.name.replace("#", "s")] = index;
        }
    }


    /*
    static fb = 12;
    static cb = 13;  // use b rather than cb
    static gb = 2;
    static db = 3;
    static ab = 4;
    static eb = 5;
    static bb = 6;
    static f = 7;
    static c = 8;
    static g = 9;
    static d = 10;
    static a = 11;
    static e = 12;
    static b = 13;
    static fs = 14;
    static cs = 15;
    static gs = 16;
    static ds = 5;   // use bb rather than d#
    static as = 6;   // use bb rather than a#
    static es = 7;   // use f rather than e#
    static bs = 8;   // use c rather than b#
    */


    /* two octaves of semitones (0-24) starting at c */
    static pitchClassBySemitone =
    [
        this.c, this.db, this.d, this.eb, 
        this.e,  this.f, this.gb, this.g, 
        this.ab, this.a, this.bb, this.b, 
        this.c, this.cs, this.d, this.ds,  
        this.e, this.f, this.fs, this.g, 
        this.gs, this.a, this.as, this.b, 
        this.bs 
    ];

    /**
     * Numbers used for key-independent pitches (e.g. in cell display).
     */
    static numbers = [
        "3", "7", "b5", "b2", "b6", "b3", "b7", 
        "4", "1", "5", "2", "6", "3", "7", 
        "#4", "#1", "#5", "#2", "#6", "4", "1"  
    ];

    // In the table below, columns are tonalities, while rows correspond to transpositions of
    // the tonality by some number of semitones (0-11).  This is an attempt to get sharps and
    // flats to be coordinated when transposing.  We don't want to see sharps and flats mixed
    // in the same column.
    static upTranspositions = 
    [
    /* 0*/ [
        this.fb, this.cb, this.gb, this.db, this.ab, this.eb, this.bb,  
        this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  this.b, 
        this.fs, this.cs, this.gs, this.ds, this.as, this.es, this.bs,
    ],
    /*1*/ [ 
        this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  this.b, 
        this.fs, this.cs, this.gs, this.ds, this.as, this.es,  this.bs,  
        this.g,  this.d,  this.a,  this.e,  this.b, this.fs, this.cs,
    ],
    /*2*/ [
        this.fs, this.cs, this.gs, this.ds, this.as,  this.f,  this.c,  
        this.g,  this.d,  this.a,  this.e,  this.b, this.fs, this.cs, 
        this.gs, this.ds, this.as, this.es,  this.bs,  this.g,  this.d,
    ],
    /*3*/ [ 
        this.g,  this.d,  this.a,  this.e,  this.b, this.fs, this.cs, 
        this.gs, this.ds, this.as,  this.f,  this.c,  this.g,  this.d,  
        this.a,  this.e,  this.b, this.fs, this.cs, this.gs, this.ds,
    ],
    /*4*/ [
        this.gs, this.ds, this.as,  this.f,  this.c,  this.g,  this.d,  
        this.a,  this.e,  this.b, this.fs, this.cs, this.gs, this.ds, 
        this.as, this.es,  this.bs,  this.g,  this.d,  this.a,  this.e,
    ],
    /*5*/ [ 
        this.a,  this.e,  this.b, this.fs, this.cs, this.gs, this.ds, 
        this.as, this.es,  this.c,  this.g,  this.d,  this.a,  this.e,  
        this.b, this.fs, this.cs, this.gs, this.ds, this.as, this.es,
    ],
    /*6*/ [
        this.as,  this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  
        this.b, this.fs, this.cs, this.gs, this.ds, this.as,  this.es, 
        this.bs,  this.g,  this.d,  this.a,  this.e,  this.b, this.fs,
    ],
    /*7*/ [ 
        this.b, this.fs, this.cs, this.gs, this.ds, this.as,  this.f,  
        this.c,  this.g,  this.d,  this.a,  this.e,  this.b, this.fs, 
        this.cs, this.gs, this.ds, this.as, this.es,  this.bs,  this.g,
    ],
    /*8*/ [ 
        this.c,  this.g,  this.d,  this.a,  this.e,  this.b, this.fs, 
        this.cs, this.gs, this.ds, this.as, this.es,  this.bs,  this.g,  
        this.d,  this.a,  this.e,  this.b, this.fs, this.cs, this.gs,
    ],
    /*9*/ [
        this.cs, this.gs, this.ds, this.as,  this.f,  this.c,  this.g,  
        this.d,  this.a,  this.e,  this.b, this.fs, this.cs, this.gs, 
        this.ds, this.as, this.es,  this.bs,  this.g,  this.d,  this.a,
    ],
    /*10*/ [ 
        this.d,  this.a,  this.e,  this.b, this.fs, this.cs, this.gs, 
        this.ds, this.as,  this.f,  this.c,  this.g,  this.d,  this.a,  
        this.e,  this.b, this.fs, this.cs, this.gs, this.ds, this.as,
    ],
    /*11*/ [
        this.ds, this.as,  this.f,  this.c,  this.g,  this.d,  this.a,  
        this.e,  this.b, this.fs, this.cs, this.gs, this.ds, this.as, 
        this.es,  this.bs,  this.g,  this.d,  this.a,  this.e,  this.b,
    ]
    ];

    static downTranspositions = 
    [
    /*0*/ [
        this.fb, this.cb, this.gb, this.db, this.ab, this.eb, this.bb,  
        this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  this.b, 
        this.fs, this.cs, this.gs, this.ds, this.as, this.es, this.bs
    ],
    /*1*/ [
        this.eb, this.bb,  this.f,  this.c,  this.g,  this.d,  this.a,  
        this.e,  this.b, this.gb, this.db, this.ab, this.eb, this.bb,  
        this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  this.b,
    ],
    /*2*/ [ 
        this.d,  this.a, this.fb, this.cb, this.gb, this.db, this.ab, 
        this.eb, this.bb,  this.f,  this.c,  this.g,  this.d,  this.a,  
        this.e,  this.b, this.gb, this.db, this.ab, this.eb, this.bb
    ],
    /*3*/ [
        this.db, this.ab, this.eb, this.bb,  this.f,  this.c,  this.g,  
        this.d,  this.a,  this.e,  this.b, this.gb, this.db, this.ab, 
        this.eb, this.bb,  this.f,  this.c,  this.g,  this.d,  this.a
    ],
    /*4*/ [ 
        this.c,  this.g,  this.d,  this.a, this.fb, this.cb, this.gb, 
        this.db, this.ab, this.eb, this.bb,  this.f,  this.c,  this.g,  
        this.d,  this.a,  this.e,  this.b, this.gb, this.db, this.ab
    ],
    /*5*/ [
        this.cb, this.gb, this.db, this.ab, this.eb, this.bb,  this.f,  
        this.c,  this.g,  this.d,  this.a,  this.e,  this.b, this.gb, 
        this.db, this.ab, this.eb, this.bb,  this.f,  this.c,  this.g
    ],
    /*6*/ [
        this.bb,  this.f,  this.c,  this.g,  this.d,  this.a,  this.e, 
        this.cb, this.gb, this.db, this.ab, this.eb, this.bb,  this.f,  
        this.c,  this.g,  this.d,  this.a,  this.e,  this.b, this.gb
    ],
    /*7*/ [ 
        this.a, this.fb, this.cb, this.gb, this.db, this.ab, this.eb, 
        this.bb,  this.f,  this.c,  this.g,  this.d,  this.a,  this.e,  
        this.b, this.gb, this.db, this.ab, this.eb, this.bb,  this.f
    ],
    /*8*/ [
        this.ab, this.eb, this.bb,  this.f,  this.c,  this.g,  this.d,  
        this.a,  this.e,  this.b, this.gb, this.db, this.ab, this.eb, 
        this.bb,  this.f,  this.c,  this.g,  this.d,  this.a,  this.f
    ],
    /*9*/ [ 
        this.g,  this.d,  this.a, this.fb, this.cb, this.gb, this.db, 
        this.ab, this.eb, this.bb,  this.f,  this.c,  this.g,  this.d,  
        this.a,  this.e,  this.b, this.gb, this.db, this.ab, this.eb
    ],
    /*10*/ [
        this.gb, this.db, this.ab, this.eb, this.bb,  this.f,  this.c,  
        this.g,  this.d,  this.a,  this.e,  this.b, this.gb, this.db, 
        this.ab, this.eb, this.bb,  this.f,  this.c,  this.g,  this.d
    ],
    /*11*/ [ 
        this.f,  this.c,  this.g,  this.d,  this.a, this.fb, this.cb, 
        this.gb, this.db, this.ab, this.eb, this.bb,  this.f,  this.c,  
        this.g,  this.d,  this.a,  this.e,  this.b, this.gb, this.db
    ]
    ];
    

}

export default PitchClass;