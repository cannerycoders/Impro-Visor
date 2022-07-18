// Duration captures and de/serializes musical duration.
// Durations are appended to notes 
//  '+' is used in notes to signify octave.  
//  '+' is also appended to 
// eg f#+4+4 means (4+4) (2 quarter notes)
// Example duration: 2.+8/3+32  (see Note.getDurationString)
//          means: a dotted halfnote plus 1/8*1/3 + 1/32

/* From leadheet help: 
   The duration of a note is specified by numbers:
    1  = whole note
    2  = half note
    4  = quarter note
    8  = eighth note
    16 = sixteenth note
    32 = thirty-second note

    Following a number by a dot (.) multiplies the value by 1.5. 
    Following a number by /3 gives the value of a triplet (2/3 of the original value).
    Durations can be augmented by following a basic duration with a plus (+)
      then the added duration.
    If no duration is specified, an eighth note will be used.

    Examples:
    a8    an 'a' above middle c, eighth note
    eb8   an e-flat above middle c, eighth note
    d+4   a 'd' in the second octave above middle c, quarter note
    g-4/3 a 'g' in the octave below middle c, quarter-note triplet
    c#4.  a c-sharp in the octave above middle c, dotted quarter note
    f+2+8 an f in the second octave above middle c, half note plus an eighth
*/
import {PitchClass} from "./pitchclass.js";
import {Constants} from "../constants.js";

const sTestDur = /^[\d\.\/\+]*$/;  // list of valid chars
const sParseDur = /^(\d*)\/*(\d*)(\.*)$/; // parse 3 fields (num, denom, mult)
const sDefaultNote = 8;

export class Duration
{
    static isDuration(str)
    {
        // charcodes: 0-9: 48-57
        return sTestDur.test(str);
    }

    static durationFromLeadsheet(str)
    {
        let len = str.length;
        if(len == 0) return 0;
        let c = str.charCodeAt(0);
        if(c == Constants.RESTCHAR)
        {
            let duration = this.getDuration(str.slice(1));
            return duration;
        }
        if(!PitchClass.isValidPitchStart(c))
        {
            return 0;
        }

        // most of our job is just to skip those characters that are part
        // of the pitch description leaving index to point to the start
        // of the duration req.
        let index = 1;
        if( index < len )
        {
            let second = str.charCodeAt(1);
            if(second == Constants.SHARPCHAR || second == Constants.FLATCHAR)
                index++;
        }

        // Check for any octave shifts specified in the notation
        let more = true;
        while(index < len && more)
        {
            switch( str.charCodeAt(index) )
            {
            case Constants.PLUSCHAR:
                index++;
                break;
            case Constants.MINUSCHAR:
                index++;
                break;
            case Constants.UCHAR:
                index++;
                break;
            default:
                more = false;
            }
        }
        return this.getDuration(str.slice(index));
    }

    /**
     * Use this to get duration from duration string such as
     * "4+8" as in leadsheet notation, except that a null 
     * string returns 0 rather than the default value.
     */
    static getDuration0(item)
    {
        let d = item.trim();
        if(d == "")
            return 0;
        else
            return this.getDuration(d);
    }
  
    /**
     * This method provides part of the functionality of noteFromLeadhsheet,
     * namely getting the duration part of a note or rest.
     * Some durations have *many* + extensions like:
     *   r1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1+1
     *  apparently valid fields:
     *      1.  (means 1.5)
     *     3/8
     */
    static getDuration(item)
    {
        let len = item.length;
        let totdur = 0; 
        if(len > 0)
        {
            let fields = item.split("+");
            for(let f of fields)
            {
                // Check for zero by trying to convert to a number.
                // If conversion is unsuccessful or value is 0, 0 will be returned
                let slots = Constants.WHOLE;
                let match = sParseDur.exec(f); // three r-matches at [1-3]
                if(!match)
                {
                    console.log("No match for " + f);
                    continue;
                }
                let nfrac = match[1] || sDefaultNote; // looks like numerator but its not
                let denom = match[2] || 1;
                let dots = match[3] ? match[3].length : 0;
                if(denom > 1)
                {
                    // tuplet
                    // 1. Quarter note triplet: A quarter note triplet consists 
                    //    of three-quarter notes over the same amount of time 
                    //    as two-quarter notes or a single half note. Quarter 
                    //    note triplets are known as crotchet triplets in Britain.
                    // 2. Eighth note triplet: An eighth note triplet contains 
                    //    three eighth notes in the space normally occupied by 
                    //    two eighth notes or a single quarter note. Eight note 
                    //    triplets are known as quaver triplets in Britain.
                    // 3. Sixteenth note triplet: A sixteenth note triplet 
                    //    contains three sixteenth notes over the span of two 
                    //    sixteenth notes or a single eighth note. Sixteenth 
                    //     note triples are known as semiquaver triplets in Britain.
                    slots *= (denom - 1);
                }
                let thisdur = slots / (nfrac * denom);
                let incr = thisdur;
                while(dots != 0)
                {
                    incr /= 2;
                    thisdur += incr;
                    dots--;
                }
                totdur += thisdur;
            }
        }
        if(totdur == 0)
            totdur = Constants.DEFAULT_DURATION;
        return totdur;
    }
}

export default Duration;
