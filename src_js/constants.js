const restCC = "r".charCodeAt(0);
const sharpCC = "#".charCodeAt(0);
const flatCC = "b".charCodeAt(0);
const plusCC = "+".charCodeAt(0);
const minusCC = "-".charCodeAt(0);
const uCC = "u".charCodeAt(0);

const sBeat = 120;
const sWhole = 4 * sBeat; // selected so smallest unit (incl triplets) is integral

export class Constants
{
    /* we use getters to express const-ness... */
    /*
    static get RESTSTR() { return "r"; } 
    static get RESTCHAR() { return restCC;  }
    static get FLATCHAR() { return flatCC; }
    static get SHARPCHAR() { return sharpCC; }
    static get PLUSCHAR() { return plusCC; }
    static get MINUSCHAR() { return minusCC; }
    static get UCHAR() { return uCC; }
    */
    
    static RESTSTR = "r";
    static RESTCHAR = "r".charCodeAt(0);
    static SHARPCHAR = "#".charCodeAt(0);
    static FLATCHAR = "b".charCodeAt(0);
    static PLUSCHAR = "+".charCodeAt(0);
    static MINUSCHAR = "-".charCodeAt(0);
    static UCHAR = "u".charCodeAt(0);
    static FLAT_SHARP = ["#", "b"];
    static CROOT = "C";
    static EIGHTH_REST_STRING = "r8";

    static IsFlatOrSharp(c) { return this.FLAT_SHARP.indexOf(c) != -1; }

    static SEMITONES = 12;
    static MODF = 5;
    static MODC = 0;
    static MODG = 7;
    static MODD = 2;
    static MODA = 9;
    static MODE = 4;
    static MODB = 11;

    static ACCIDENTALS = 
    {
        natural: 0,
        sharp: 1,
        flat: 2,
        nothing: 3
    };

    static NoteTypes =
    {
        ChordTone: 0,
        ColorTone: 1,
        ApproachTone: 2,
        ForeignTone: 3
    };

    
    static BEAT = 120; // slots in a beat
    static WHOLE = 4 * this.BEAT;  // 480, "ticks"
    static THIRD_BEAT = 40;
    static HALF_BEAT = 60;
    static SIXTH_BEAT = 20;
    static DEFAULT_DURATION = this.HALF_BEAT;	// eighth note value

    static DUR =
    {
        WHOLE                : sWhole, 
        HALF                 : sWhole/2,              // 240
        QUARTER              : sWhole/4,              // 120
        EIGHTH               : sWhole/8,              //  60
        SIXTEENTH            : sWhole/16,             //  30
        THIRTYSECOND         : sWhole/32,             //  15
    
        HALF_TRIPLET         : 2*(sWhole/2)/3,        // 160
        QUARTER_TRIPLET      : 2*(sWhole/4)/3,        //  80
        EIGHTH_TRIPLET       : 2*(sWhole/8)/3,        //  40
        SIXTEENTH_TRIPLET    : 2*(sWhole/16)/3,       //  20
        THIRTYSECOND_TRIPLET : 2*(sWhole/32)/3,       //  10

        QUARTER_QUINTUPLET      : 4*(sWhole/4)/5,      //  96
        EIGHTH_QUINTUPLET       : 4*(sWhole/8)/5,      //  48
        SIXTEENTH_QUINTUPLET    : 4*(sWhole/16)/5,     //  24
        THIRTYSECOND_QUINTUPLET : 4*(sWhole/32)/5,     //  12

        DOTTED_HALF           : 3*(sWhole/2)/2,        // 360
        DOTTED_QUARTER        : 3*(sWhole/4)/2,        // 180
        DOTTED_EIGHTH         : 3*(sWhole/8)/2,        //  90
        DOTTED_SIXTEENTH      : 3*(sWhole/16)/2,       //  45

        SIXTIETH              : 8,                   //   8
        ONETWENTIETH          : 4,                   //   4
        TWOFORTIETH           : 2,                   //   2
        FOUREIGHTIETH         : 1,                   //   1
    };

    constructor() {}
}

export default Constants;