// Terminals are the "end-of-the-line" for a grammar
// This file is a namespace for terminal-identification methods

/* abstract notes
        'A': // Approach
        'C': // Chord
        'H': // Helpful = Chord or Color
        'L': // Color
        'R': // Rest
        'S': // Scale
        'X': // Arbitrary
        'Y': // Outside
 */
import Duration from "../data/duration.js";

const sAbstractNoteCodes = "ACHLRSXY".split("").map(v => v.charCodeAt(0));

export class Terminals
{

    static isTerminal(str)
    {
        return this.isAbstractNote(str)
                || this.isScaleDegree(str) 
                || this.isSlope(str) 
                || this.isTriadic(str) 
                || this.isWrappedTerminal(str);

    }

    /**
     * An abstract note is a note of the type in the original abstract melody.
     * This means one of the letters {A, C, H, L, R, S, X, Y} followed by
     * a duration expression, such as 2+4.
     * @param ob
     * @return 
     */
    static isAbstractNote(str)
    {
        if(typeof(str) !== "string") return false;
        if(str.length == 0) return false;
        let chr = str.charCodeAt(0);
        if(sAbstractNoteCodes.indexOf(chr) == -1) return false;
        return Duration.isDuration(str.slice(1));
    }

    /**
     * A wrapped terminal is an abstract note as a single element of a list
     * S expression.
     */
    static isWrappedTerminal(pl)
    {
        if(Array.isArray(pl))
        {
            if(pl.length == 1)
                return this.isAbstractNote(pl[0]); 
            // other arrays (slope, triadic, etc length > 1)
        }
        else
        if(typeof(pl) === "string") 
            return this.isAbstractNote(pl);
        return false;
    }

    static isSlope(pl)
    {
        if(!Array.isArray(pl) || pl.length == 0) return false;
        return pl[0] == "slope";
    }

    static isTriadic(pl)
    {
        if(!Array.isArray(pl) || pl.length != 3) return false;
        return pl[0] == "triadic";
    }

    static isScaleDegree(pl)
    {
        if(!Array.isArray(pl) || pl.length != 3) return false;
        return (pl[0] == "X");
        // XXX: could also validate types for pl[1] and pl[2]
    }

    /**
     * Get the duration of various terminals.
     * This is used in calculating chordSlot, for example.
     * @param ob
     * @return 
     $j*/
    static getDuration(plistOrStr)
    {
        //System.out.print("duration of " + ob + " = ");
        let result = 0;
        if(Array.isArray(plistOrStr))
        {
            if(this.isScaleDegree(plistOrStr))
                result = Duration.getDuration(plistOrStr[2]);
            else
            if(isSlope(plistOrStr))
            {
                // Get the tail from the fourth element on
                let body = plistOrStr.slice(3); // Polylist body = ((Polylist)ob).rest().rest().rest();
                let sum = 0;
                for(let b of body)
                    sum += this.getDuration(b);
                result = sum;
            }
            else 
            if(isTriadic(ob))
            {
                result = Duration.getDuration(plistOrStr[1]);
            }
        }
        else
        {
            console.assert(typeof(plistOrStr) == "string");
            result = Duration.getDuration(plistOrStr);
        }
    
        //System.out.println(result);
        return result;
    }

    static getDurationAbstractMelody(pl) // unused
    {
        let duration = 0;
        for(l of pl)
            duration += this.getDuration(l);
        return duration;
    }

    static truncateAbstractMelody(pl, desiredDuration)
    {
        let duration = 0;
        let result = [];
        for(l of pl)
        {
            let dur = this.getDuration(l);
            if( duration + dur > desiredDuration )
                break;
            result.push(first);
            duration += dur;
        }
        return buffer.toPolylist();    
    }    
}

export default Terminals;