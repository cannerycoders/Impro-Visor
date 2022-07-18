import {Constants} from "../constants.js";
import {PitchClass} from "./pitchclass.js";
import {Duration} from "./duration.js";

const sDefaultPitchSymbol = "c+";
const sDefaultDuration = Constants.BEAT/2;
const sDefaultVolume = 127; // ie max
const sDefaultProbability = 1.0;
const sRestPitch = -1;

export class NoteSymbol
{
    static makeList(slist)
    {
        return slist.map((s) => NoteSymbol.make(s));

    }
    static make(str, transposition=0)
    {
        let lstr = str.toLowerCase();
        let fchr = lstr.charCodeAt(0);
        if(fchr == Constants.RESTCHAR)
            return new NoteSymbol()
        if(!PitchClass.isValidPitchStart(fchr))
            return null;
        let index = 1;
        let notebase = lstr[0];
        let octave = 0;
        let duration;
        if(index < lstr.length)
        {
            let nchr = lstr.charCodeAt(1); // look for sharp or flat
            if(nchr == Constants.FLATCHAR || nchr == Constants.SHARPCHAR)
            {
                notebase += lstr[1];
                index++;
            }
            while(index < lstr.length)
            {
                let x = lstr.charCodeAt(index);
                if(x == Constants.PLUSCHAR || x == Constants.UCHAR)
                    octave++;
                else
                if(x == Constants.MINUSCHAR)
                    octave--;
                else
                    break;
                index++;
            }
            if(index < lstr.length)
                duration = Duration.getDuration(lstr.slice(index));
        }
        let pc = PitchClass.get(notebase);
        if(!pc)
            return null;
        else
            return new NoteSymbol(pc, octave, duration);
    }

    /* ----------------------------------------------------------- */
    constructor(pitchClass, octave, duration, volume)
    {
        this.pitchClass = pitchClass || sRestPitch;
        this.octave = octave || 0;
        this.duration = duration || sDefaultDuration;
        this.volume = volume || sDefaultVolume;
        this.probability = sDefaultProbability;
    }

    isRest()
    {
        return this.pitchClass === sRestPitch;
    }

    getSemitones() 
    { 
        return this.isRest() ? -1 : this.pitchClass.getSemitones(); 
    }

    enharmonic(other)
    {
        return this.getSemitones() == other.getSemitones();
    }

}

export default NoteSymbol;