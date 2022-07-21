import {Pattern} from "./pattern.js";
import {Duration} from "../../data/duration.js";
import {NoteSymbol} from "../../data/noteSymbol.js";
import {VolumeSymbol} from "../../data/volumeSymbol.js";
import {MelodySymbol} from "../../data/melodySymbol.js";
import {Constants} from "../../constants.js";

const sRuleTypes = [
    "X", // Pitch: 0
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "10",
    "B", // Bass: 11
    "C", // chord: 12
    "S", // scale: 13
    "A", // approach: 14
    "N", // next: 15,
    "R", // rest: 16
    "=", // equal 17,
    "U", // Up : 18,
    "D", // Down: 19
    "V", // volume: 20
];

const sIRule =  // these indicies are stored in rules list, match sRuleTypes
{
    PITCH: 0,
    BASS: 11,
    CHORD: 12,
    SCALE: 13,
    APPROACH: 14,
    NEXT: 15,
    REST: 16,
    EQUAL: 17,
    UP: 18,
    DOWN: 19,
    VOLUME: 20,
};

const sBaseRuleCodes = ["A", "B", 'C', 'N', 'R', 'S', 'V', 'X', '='].map(v => v.charCodeAt(0));
const sFlatten = 100;
const sSharpen = 200;
const sFlat_Str = "b";
const sSharp_Str = "#";
const sNotePlus = "U";
const sNoteMinus = "d";
const sStay = 0;
const sBigRise = 6; // unused, "not-near"
const sMargin = 11; // internal bass can exceed limits by this
const sSoftMargin = 6;

export class BassPattern extends Pattern
{
    /* -------------------------------------------------------------- */
    /**
     * constructor implements static makePattern
     * @param {Style} style 
     * @param {Array} plist 
     * @param {Object} definedRules - ie style's bassPatternMap
     */
    constructor(style, plist, definedRules)
    {
        super(style);
        this.definedRules = definedRules;

        this.patternName = null; // optional
        this.rules = []; // indices into ruleTypes array
        this.durations = []; // string
        this.modifiers = []; // string

        // [bass-pattern, [name], [rules, B4, S4, C4, V90, A4], [weight, 10.0]
        // [bass-pattern, [rules, B4/3+16+64/3, R8/3+64/3, B8, [X, 5, 4+32/3], R8/3+32/3, A8, R16/3], [weight, 50]]
        if(!plist) return; // empty

        for(let p of plist)
        {
            switch(p[0])
            {
            case "name":
                if(p.length == 1)
                    this.patternName = null;
                else
                    this.patternName = p.slice(1).join(" ");
                break;
            case "rules":
                for(let r of p.slice(1))
                {
                    if(!Array.isArray(r))
                    {
                        let rule = r[0];
                        let duration = r.slice(1);
                        let ruleCode = rule.charCodeAt(0);
                        if(sBaseRuleCodes.indexOf(ruleCode) != -1)
                            this.addRule(rule, duration);
                        else
                            console.warn("BassPattern unrecognized rule: " + rule);
                    }
                    else
                    { 
                        // a pitch
                        if(r.length >= 3 && r[0] == "X")
                        {
                            let rule = `${r[1]}`; // toString
                            let duration = `${r[2]}`; // toString
                            let modifier = `${r[3]}`; // may be undefined
                            this.addRule(rule, duration, modifier);
                        }
                        else
                            console.warn("BassPattern unrecognized segment " + r);
                    }
                }
                break;
            case "weight":
                this.weight = Number(p[1]);
                break;
            case "use": // this theoretically allows for references by name
            default:
                console.log("BassPattern unexpected field in definition " + plist[0]);
                break;
            }
        }
    }

    getDefinedRules() { return this.definedRules; }
    setDefinedRules(map)
    {
        this.definedRules = map;
    }

    addRule(rule, duration, modifier)
    {
        if(rule.length == 2) // eg b5, #4
        {
            let r = rule[1];
            let code = rule.charCodeAt(0);
            if(code == Constants.FLATCHAR)
                this.rules.push(sFlatten + sRuleTypes.indexOf(r));
            else
            if(code == Constants.SHARPCHAR)
                this.rules.push(sSharpen + sRuleTypes.indexOf(r));
            else
            {
                console.warn("BassPattern unknown rule " + rule);
                return;
            }
        }
        else
        {
            console.assert(rule.length == 1);
            this.rules.push(sRuleTypes.indexOf(rule));
        }
        this.durations.push(duration);
        this.modifiers.push(modifier);
    }

    /**
     * @returns total duration of this pattern
     */
    getDuration() // override
    {
        let duration = 0;
        for(let dstr of this.durations)
            duration += Duration.getDuration0(dstr);
        return duration;
    }

    /**
     * Realizes the Pattern into a sequencable Polylist
     *
     * @param chord the ChordSymbol to use for the bassline
     * @param nextChord the ChordSymbol that comes next in the progression
     * @param lastNote a NoteSymbol containing the previous bassline note
     * @return A list of NoteSymbol objects that make up the bassline. 
     */
    applyRules(chord, nextChord, lastNote)
    {
        let basslineSegment = []; // array of NoteSymbol
        let chordRoot = chord.getRootString(); 
        let chordForm = chord.getForm();
        let key = chordForm.getKey(chordRoot);
        let rise = PitchClass.findRise(chordRoot);

        // indicator for directional placement
        let indicator = sStay;
        let volume = 127;

        for(let i=0;i<this.rules.length; this.rules) // list of indices
        {
            let rule = this.rules[i];
            let duration = this.durations[i];
            let modifier = this.modifiers[i];
            let melodySymbol;
    //System.out.println("applying bass rule " + rule + ", duration = " + duration + ", modifier = " + modifier);
            switch(rule)
            {
                case sIRule.VOLUME:
                {
                    melodySymbol = new VolumeSymbol(duration);
                    //System.out.println("creating VolumeSymbol: " + melodySymbol);
                    break;
                }

                case sIRule.PITCH: // Allow X for bass too, 
                // as a convenience in cutting and pasting in editor
                case sIRule.BASS:
                {
                    melodySymbol = new NoteSymbol(chord.getBass());
                    break;
                }

                case sIRule.NEXT:
                {
                    // FIX: This may be broken (octave jumps). Please check
                    let noteSymbol = new NoteSymbol(nextChord.getBass());
                    if(i+1 < this.rules.length) // has-next
                    {
                        melodySymbol = noteSymbol;
                    }
                    else
                    {
                        melodySymbol = new NoteSymbol(
                                noteSymbol.getPitchClass(),
                                noteSymbol.getOctave(),
                                Duration.getDuration0(duration));
                    }
                    basslineSegment = [duration, melodySymbol];
                    return basslineSegment;
                }

                case sIRule.CHORD:
                {
                    let chordTones = chordForm.getSpell(chordRoot, key);
                    if(chordTones.length() > 1)
                        chordTones = lastNote.enhDrop(chordTones);
                    melodySymbol = this.getRandomItem(chordTones);
                    break;
                }

                case sIRule.SCALE:
                {
                    let scales = chordForm.getScales();
                    if(!scales || scales.length == 0)
                    {
                        let chordTones = chordForm.getSpell(chordRoot, key);
                        if(chordTones.length() > 1)
                            chordTones = lastNote.enhDrop(chordTones);

                        melodySymbol = this.getRandomItem(chordTones);
                        break;
                    }
                    let scale = scales.first();

                    let tonic = NoteSymbol.makeNoteSymbol(scale[0]);
                    let scaleType = Advisor.Get().concatListWithSpaces(scale.rest());
                    let scaleForm = Advisor.getScale(scaleType);
                    let tones = scaleForm.getSpell(tonic);
                    tones = NoteSymbol.transposeNoteSymbolList(tones, rise);
                    // huh? tones = tones.reverse().rest().reverse();
                    tones.pop(); // above huh? interpretted as pop_back

                    let seconds = this.getIntervals(2, tones, lastNote);
                    let thirds = this.getIntervals(3, tones, lastNote);
                    tones = seconds.push(...thirds);
                    if(tones.length() > 1)
                        tones = lastNote.enhDrop(tones);
                    melodySymbol = this.getRandomItem(tones);
                    break;
                }

                case sIRule.APPROACH:
                {
                    let noteSymbol = new NoteSymbol(nextChord.getBass());
                    let approach = [noteSymbol.transpose(1),
                                    noteSymbol.transpose(-1)];
                    if(approach.length() > 1)
                        approach = lastNote.enhDrop(approach);

                    melodySymbol = this.getRandomItem(approach);
                    break;
                }

                case sIRule.REST:
                {
                    melodySymbol = NoteSymbol.makeNoteSymbol("r");
                    break;
                }

                case sIRule.EQUAL:
                {
                    melodySymbol = new NoteSymbol(lastNote); 
                    break;
                }

                default: 
                {                             
                    // higher than 99 means flat/sharp
                    if((rule > 0 && rule < 11) || rule > 99)
                    {
                        let scales = chordForm.getScales();
                        if(!scales || !scales.length)
                        {
                            let chordTones = chordForm.getSpell(chordRoot, key);
                            if(chordTones.length() > 1)
                                chordTones = lastNote.enhDrop(chordTones);

                            melodySymbol = this.getRandomItem(chordTones);
                            break;
                        }

                        let scale = scales[0];

                        let tonic = NoteSymbol.makeNoteSymbol(scale[0]);
                        let scaleType = Advisor.Get().concatListWithSpaces(scale.rest());
                        let scaleForm = Advisor.getScale(scaleType);
                        let tones = scaleForm.getSpell(tonic);
                        tones = NoteSymbol.transposeNoteSymbolList(tones, rise);
                        // tones = tones.reverse().rest().reverse();
                        tones.pop(); // pop_back

                        // flattened notes
                        if(rule > sIRule.FLATTEN && rule < sIRule.FLATTEN + 11)
                        {
                            rule = rule - sIRule.FLATTEN;
                            let noteSymbol = this.getInterval(rule, tones);
                            melodySymbol = noteSymbol.transpose(-1);
                        }   // sharpened notes
                        else 
                        if(rule > sIRule.SHARPEN && rule < sIRule.SHARPEN + 11)
                        {
                            rule = rule - sIRule.SHARPEN;
                            let noteSymbol = this.getInterval(rule, tones);
                            melodySymbol = noteSymbol.transpose(1);
                        }
                        else
                        {
                            melodySymbol = this.getInterval(rule, tones);
                        }
                    }
                    else
                    {
                        melodySymbol = new NoteSymbol(chord.getBass());
                    }
                    break;
                } // end of default-case
            } // end of rule switch
            
            if(melodySymbol)
            {
                if(MelodySymbol.IsNoteSymbol(melodySymbol)) // instanceOf NoteSymbol
                {
                    let noteSymbol = melodySymbol;
                    if(!noteSymbol.isRest() && rule != sIRule.EQUAL)
                    {
                        // System.out.println("Original melodySymbol is " + melodySymbol.getMIDI() );

                        // Why -24??

                        noteSymbol = noteSymbol.transpose(-24);
                        //pitch = placePitchNear(melodySymbol, lastNote, style);

                        if(modifier == "U")
                            noteSymbol = this.placePitchAbove(noteSymbol, lastNote);
                        else 
                        if(modifier == "D")
                            noteSymbol = this.placePitchBelow(noteSymbol, lastNote);
                        else 
                        if(modifier == "DD")
                            noteSymbol = this.placePitchOctaveBelow(noteSymbol, lastNote);
                        else
                        {
                            noteSymbol = this.placePitchNear(noteSymbol, lastNote, style);
                            noteSymbol = this.pressure(noteSymbol, style);
                        }
                    }

                    let note = new NoteSymbol(
                            noteSymbol.getPitchClass(),
                            noteSymbol.getOctave(),
                            Duration.getDuration0(duration),
                            volume);

                    basslineSegment.add(note);

                    if(!note.isRest())
                        lastNote = note;
                }
                else 
                if(MelodySymbol.IsVolumeSymbol(melodySymbol))
                    basslineSegment.add(melodySymbol);
                else
                    console.assert(false);
            }
        //System.out.println("rule = " + ruleTypes[rule] + " melodySymbol = " + melodySymbol);
        } // end of rules loop
        
        return basslineSegment;
    }

    /**
     * Returns a random item from a given Polylist.
     * @param L         a Polylist to return an item from
     * @return a random Object from the Polylist
     */
    getRandomItem(L)
    {
        L = this.filterOutStrings(L);
        let i = Math.floor(Math.random() * L.length);
        return L[i];
    }

    filterOutStrings(L)
    {
        let nl = [];
        for(let l of L)
        {
            if(typeof(l) != "string")
                nlpush(l);
        }
        return nl;
    }

    /**
     * Takes a list of notes and an index interval and returns the
     * NoteSymbol at that index.
     * @param interval  an int representing the index interval to access
     * @param notes     a Polylist of NoteSymbol objects
     * @return a NoteSymbol at the proper index interval
     */
    getInterval(interval, notes)
    {
        if(interval > 10)
            interval = interval - 10;
        interval = (interval % 11 - 1) % notes.length(); // FIX!
        return notes[interval]; // (NoteSymbol)notes.nth(interval);
    }
    
    /**
     * Returns the notes that are a certaing index interval away from a given
     * root.
     * @param interval  an int representing the index interval to access
     * @param notes     a Polylist of NoteSymbol objects
     * @param root      a NoteSymbol being the reference point to index from
     * @return a Polylist of NoteSymbols representing the index interval in
     *         either direction from the root
     */
    getIntervals(interval, notes, root)
    {
        interval = interval % 7 - 1;

        let rootPos = -1;
        for(let i = 0; i < notes.length; i++)
        {
            let note = notes[i];
            if(MelodySymbol.IsNoteSymbol(note) && note.enharmonic(root))
            {
                rootPos = i;
                break;
            }
        }
        console.assert (rootPos != -1);
        let a = notes[(rootPos+interval) % notes.length];
        let b = notes[(rootPos-interval + notes.length) % notes.length];
        // L = L.cons(notes.nth((rootPos + interval) % notes.length()));
        // L = L.cons(notes.nth((rootPos - interval + notes.length()) % notes.length()));
        return [a, b]
    }

    placePitchAbove(pitch, base)
    {
        let semitones = base.getSemitonesAbove(pitch);
        return base.transpose(semitones);
    }
  
    /**
     * Takes a melodySymbol NoteSymbol and a base NoteSymbol and transposes the
    * melodySymbol to be within the octave below the base.
    * @param melodySymbol     a NoteSymbol that is the melodySymbol to place
    * @param base      a NoteSymbol that is the base note
    * @return a NoteSymbol that is the placed melodySymbol
    */
    placePitchBelow(pitch, base)
    {
        // Note the role reversal of melodySymbol and base from the previous method
        let semitones = pitch.getSemitonesAbove(base);
        return base.transpose(-semitones);
    }
  
    placePitchOctaveBelow(pitch, base)
    {
        // Note the role reversal of melodySymbol and base from the previous method
        let semitones = pitch.getSemitonesAbove(base);
        return base.transpose(-semitones).transpose(-12);
    }
  
    /**
     * Takes a melodySymbol NoteSymbol and a base NoteSymbol and transposes the
    * melodySymbol to be near the base and within the given range.
    * @param melodySymbol     a NoteSymbol that is the melodySymbol to place
    * @param base      a NoteSymbol that is the base note
    * @param low       a NoteSymbol that is the lower range
    * @param high      a NoteSymbol that is the upper range
    * @return a NoteSymbol that is the placed melodySymbol
    */
    placePitchNear(pitch, base, style)
    {
        let low = style.getBassLow();
        let high = style.getBassHigh();
        //System.out.println("placePitchNear " + melodySymbol + ", style = " + style + ", low = " + low + ", base = " + base + ", high = " + high);
        let rise = base.getSemitonesAbove(pitch);
        let note;

        // Pitch octave placement is the subject of some experimentation.
        /*  
        boolean drop_down = base.getDuration() >= BEAT || rise >= BIG_RISE;
        if( drop_down )
        {
            note = placePitchBelow(melodySymbol, base);
            if( note.getMIDI() < low.getMIDI() - MARGIN )
                {
                note = note.transpose(12);
                // note = placePitchAbove(melodySymbol, base);
                }
            //System.out.println("base = " + base + ", rise = " + rise + ", note = " + note + " below");
            }
            else
            {
            note = placePitchAbove(melodySymbol, base);
            if( note.getMIDI() > high.getMIDI() + MARGIN )
                {
                note = note.transpose(-12);
                // note = placePitchBelow(melodySymbol, base);
                }
            //System.out.println("base = " + base + ", rise = " + rise + ", note = " + note + " above");
        }
        */

        while(pitch.getMIDI() > base.getMIDI() + sMargin ||
              pitch.getMIDI() < base.getMIDI() - sMargin)
        {
            let rand = Math.random();
            if(rand < 0.5)
            {
                if(pitch.getMIDI() > base.getMIDI() + sMargin)
                    pitch = pitch.transpose(-12);
                else 
                if(pitch.getMIDI() < base.getMIDI() - sMargin)
                    pitch = pitch.transpose(12);
            }
            else
            {
                if(pitch.getMIDI() < base.getMIDI() - sMargin)
                    pitch = pitch.transpose(12);
                else 
                if(pitch.getMIDI() > base.getMIDI() + sMargin )
                    pitch = pitch.transpose(-12);
            }
            //  System.out.println("PITCH IS " + melodySymbol.getMIDI());
        }
        return pitch;
    }

    /**
     * Takes a noteSymbol and a range and transposes the
     * melodySymbol probabilistically based on its position in the range.
     * @param melodySymbol a NoteSymbol that is the melodySymbol to place
     * @param low       a NoteSymbol that is the lower range
     * @param high      a NoteSymbol that is the upper range
     * @return a NoteSymbol that is the placed melodySymbol
     */
    pressure(pitch, style)
    {
        let low = style.getBassLow();
        let high = style.getBassHigh();

        // find the center by going up from the low
        let hardmargin = high.getSemitonesAbove(low);

        // For some reason hardmargin is only half as large as I'd like
        let center = low.transpose(hardmargin);
        //  System.out.println("high " + high.getMIDI() + " low " + low.getMIDI() + " center " + center.getMIDI());

        let softmarginhigh = center.transpose(sSoftMargin);
        let softmarginlow = center.transpose(-sSoftMargin);

        // take probability linearly based on melodySymbol position in margins
        if(pitch.getMIDI() > softmarginhigh.getMIDI()) 
        {
            let numerator = pitch.getMIDI() - softmarginhigh.getMIDI();
            let denominator = high.getMIDI() - softmarginhigh.getMIDI();
            let prob = numerator / denominator;
            prob = prob*prob*prob*prob;
            let rand = Math.random();
            if(prob > rand)
                pitch = pitch.transpose(-12);
        }
        else 
        if(pitch.getMIDI() < softmarginlow.getMIDI())
        {
            let numerator = softmarginlow.getMIDI() - pitch.getMIDI();
            let denominator = softmarginlow.getMIDI() - low.getMIDI();
            let prob = numerator / denominator;
            prob = prob*prob*prob*prob;
            let rand = java.lang.Math.random();
            if(prob > rand)
                pitch = pitch.transpose(12);
        }
        //System.out.println("New melodySymbol is " + melodySymbol.getMIDI());
        return pitch;
    } // end pressure
}

export default BassPattern;