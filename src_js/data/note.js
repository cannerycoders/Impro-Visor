import {Constants} from "../constants.js";
import {IUnit} from "./iunit.js";

let sDefaultPitch = 60;
let sUndefinedPitch = -2;
let sDefaultVolume = 127;
let sDefaultRhythmValue = Constants.DEFAULT_DURATION;
let sAcc = Constants.ACCIDENTALS;
let sDur = Constants.DUR;
const sSharpInKeysAbove = [ 6, 1, 9, 3, 5, 9, 0, 9, 2, 9, 4, 9 ];	// 9 is like infinity
const sFlatInKeysBelow = [ 9, 4, 9, 2, 7, 9, 5, 9, 3, 9, 1, 6 ];
const sFlatPitchFromMidi = ["c", "db", "d", "eb", "fb", "f", "gb", "g", "ab", "a", "bb", "cb"];
const sSharpPitchFromMidi = ["b#", "c#", "d", "d#", "e", "e#", "f#", "g", "g#", "a", "a#", "b"];
const sNaturalPitchFromMidi = ["c", "c", "d", "d", "e", "f", "f", "g", "g", "a", "a", "b"];

export class Note extends IUnit
{
    static sBlackNotes = [1, 3, 6, 8, 10]; // in MIDI coords % 12

    static MakePitchSharpDur(pitch, sharp, rhythm=sDefaultRhythmValue)
    {
        return new Note(pitch, this.getSharpOrFlat(sharp), rhythm);
    }
    static MakePitchDur(pitch, rhythm=sDefaultRhythmValue)
    {
        return new Note(pitch, 
                this.sBlackNotes[pitch%12] ? sAcc.sharp : sAcc.natural,
                rhythm);
    }
    static MakeNote(pitch, nat, sharp, rhythm)
    {
        let acc = nat ? sAcc.natural : (sharp ? sAcc.sharp : sAcc.flat);
        return new Note(pitch, acc, rhythm);
    }

    static GetDurationString(value)
    {
        if(value < 0) return "";
        let saveVal = value;

        // first decomposition
        let accum = [];
        value = accumulateValue(sDur.WHOLE, value, accum, "+1");
        value = accumulateValue(sDur.HALF, value, accum, "+2");
        value = accumulateValue(sDur.QUARTER, value, accum, "+4");
        value = accumulateExactValue(sDur.QUARTER_QUINTUPLET, value, accum, "+4/5");
        value = accumulateValue(sDur.EIGHTH, value, accum, "+8");
        value = accumulateExactValue(sDur.EIGHTH_QUINTUPLET, value, accum, "+8/5");
        value = accumulateValue(sDur.SIXTEENTH, value, accum, "+16");
        value = accumulateExactValue(sDur.SIXTEENTH_QUINTUPLET, value, accum, "+16/5");
        value = accumulateValue(sDur.THIRTYSECOND, value, accum, "+32");
        value = accumulateExactValue(sDur.THIRTYSECOND_QUINTUPLET, value, accum, "+32/5");
        value = accumulateValue(sDur.HALF_TRIPLET, value, accum, "+2/3");
        value = accumulateValue(sDur.QUARTER_TRIPLET, value, accum, "+4/3");
        value = accumulateValue(sDur.EIGHTH_TRIPLET, value, accum, "+8/3");
        value = accumulateValue(sDur.SIXTEENTH_TRIPLET, value, accum, "+16/3");
        value = accumulateValue(sDur.THIRTYSECOND_TRIPLET, value, accum, "+32/3");

        // To make any residual concise
        value = accumulateValue(sDur.SIXTIETH, value, accum, "+60");
        value = accumulateValue(sDur.ONETWENTIETH, value, accum, "+120");
        value = accumulateValue(sDur.TWOFORTIETH, value, accum, "+240");
        value = accumulateValue(sDur.FOUREIGHTIETH, value, accum, "+480");
        let residue1 = value;
        let version1 = accum.join("");

        // alt decomposition
        accum = [];
        value = saveVal;
        value = accumulateValue(sDur.HALF_TRIPLET, value, buffer, "+2/3");
        value = accumulateValue(sDur.QUARTER_TRIPLET, value, buffer, "+4/3");
        value = accumulateValue(sDur.EIGHTH_TRIPLET, value, buffer, "+8/3");
        value = accumulateValue(sDur.SIXTEENTH_TRIPLET, value, buffer, "+16/3");
        value = accumulateValue(sDur.THIRTYSECOND_TRIPLET, value, buffer, "+32/3");
        value = accumulateValue(sDur.WHOLE, value, buffer, "+1");
        value = accumulateValue(sDur.HALF, value, buffer, "+2");
        value = accumulateValue(sDur.QUARTER, value, buffer, "+4");
        value = accumulateValue(sDur.EIGHTH, value, buffer, "+8");
        value = accumulateValue(sDur.SIXTEENTH, value, buffer, "+16");
        value = accumulateValue(sDur.THIRTYSECOND, value, buffer, "+32");

        // To make any residual concise
        value = accumulateValue(sDur.SIXTIETH, value, buffer, "+60");
        value = accumulateValue(sDur.ONETWENTIETH, value, buffer, "+120");
        value = accumulateValue(sDur.TWOFORTIETH, value, buffer, "+240");
        value = accumulateValue(sDur.FOUREIGHTIETH, value, buffer, "+480");
        let residue2 = value;
        let version2 = accum.join("");

        // select the shorter representation
        return version1.length <= version2.length ? version1 : version2;
    }

    static accumValue(ival, dur, accum, str)
    {
        while(dur >= ival)
        {
            accum.push(str);
            dur =- ival;
        }
        return dur;
    }

    /* ---------------------------------------------------------------------- */

    constructor(pitch, acc, rhythmValue)
    {
        this.pitch = pitch;
        this.accidental = acc; 
        this.rhythmValue = rhythmValue;
        this.volume = sDefaultVolume;
    }

    fixupKey(keySig) /* cf Part.java */
    {
        if(this.isAccidentalInKey(keySig))
        {
            // If the note shows as accidentally in the key,
            // see if toggling it will help, and if not, 
            // toggle back.
            this.toggleEnharmonic();
            if(this.isAccidentalInKey(keySig))
                this.toggleEnharmonic();
        }
    }

    isAccidentalInKey(keySig)
    {
        if(this.pitch < 0 || keySig == 0)
            return false;
        else
        {
            if(keySig > 0)
            {
                return this.accidental == sAcc.flat //|| accidental == Accidental.NATURAL)  // See if this can be fixed.
                        && keySig > sSharpInKeysAbove[this.pitch % 12];

            }
            if(keySig < 0)
            {
                return accidental == sAcc.sharp //           || accidental == Accidental.NATURAL) // See if this can be fixed.
                        && keySig <= -sFlatInKeysBelow[this.pitch % 12];
            }
        }
    }

    toggleEnharmonic()
    {
        let c = Constants;
        let pmod = this.pitch % c.SEMITONES;
        let a = this.accidental;
        if( pmod == c.MODF && a == sAcc.sharp ||
            pmod == c.MODE && a == sAcc.flat ||
            pmod == c.MODB && a == sAcc.flat ||
            pmod == c.MODC && a == sAcc.sharp )
        {
            this.accidental = sAcc.natural;
            return true;
        }
        else 
        if( a == sAcc.sharp ||
            a == sAcc.natural && (pmod == c.MODE || pmod == c.MODB) )
        {
            this.accidental = sAcc.flat;
            return true;
        }
        else 
        if( a == sAcc.flat || a == sAcc.natural && 
            (pmod == c.MODF || pmod == c.MODC) )
        {
            this.accidental = sAcc.sharp;
            return true;
        }
        return false;
    }
}

export default Note;
